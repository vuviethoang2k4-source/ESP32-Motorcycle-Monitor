/*
 * File: esp32s3_main.ino
 * Chuc nang:
 * - Ket noi ESP32 DevKit V1 voi WiFi va Blynk.
 * - Doc GPS NEO-8M de lay toc do, vi do, kinh do.
 * - Hien thi toc do hien tai va toc do cao nhat len Blynk.
 * - Reset toc do cao nhat bang nut V10 tren Blynk.
 * - Canh bao qua toc do bang Blynk notification va coi buzzer.
 * - Doc MPU6050 de chong trom 3 cap:
 *   + Lan 1: bip 1 lan
 *   + Lan 2: bip 2 lan
 *   + Lan 3: coi keu manh 5 giay + gui theft_alert kem link Google Maps
 */

#include "secrets.h"
#include "config.h"
#include "gps_module.h"
#include "mpu6050_module.h"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

BlynkTimer timer;


// ================== Bien toc do / GPS ==================

uint8_t currentSpeedKmh = 0U;
uint8_t maxSpeedKmh = 0U;
uint8_t speedLimitKmh = DEFAULT_SPEED_LIMIT_KMH;

GpsData_t latestGpsData;


// ================== Bien trang thai he thong ==================

bool guardModeEnabled = false;
bool findMotorcycleEnabled = false;

bool overspeedActive = false;
bool wasOverspeed = false;

bool theftStrongAlarmActive = false;
bool theftNotificationSent = false;

uint8_t lastHandledTamperCount = 0U;

unsigned long lastOverspeedNotifyMs = 0UL;
unsigned long theftStrongAlarmStartMs = 0UL;

static const unsigned long THEFT_STRONG_ALARM_DURATION_MS = 5000UL;


// ================== Bien giam spam Blynk ==================

String lastSystemStatusSent = "";
uint8_t lastAlertStatusSent = 255U;


// ================== Dieu khien coi / LED ==================

void buzzerOn(void)
{
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);
}


void buzzerOff(void)
{
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
}


/*
 * Coi cua ban la active HIGH:
 * GPIO25 = HIGH -> coi keu
 * GPIO25 = LOW  -> coi tat
 */
void updateLocalAlarmOutput(void)
{
    bool alarmOutputOn = false;

    if ((overspeedActive == true) ||
        (findMotorcycleEnabled == true) ||
        (theftStrongAlarmActive == true))
    {
        alarmOutputOn = true;
    }

    if (alarmOutputOn == true)
    {
        buzzerOn();
    }
    else
    {
        buzzerOff();
    }
}


void beepOnce(void)
{
    buzzerOn();
    delay(120);
    buzzerOff();
}


void beepNTimes(uint8_t times)
{
    for (uint8_t i = 0U; i < times; i++)
    {
        beepOnce();
        delay(150);
    }

    updateLocalAlarmOutput();
}


// ================== Tao link Google Maps ==================

String buildMapLink(void)
{
    if (latestGpsData.isValid == false)
    {
        return String("Chua co GPS hop le");
    }

    String mapLink = "https://maps.google.com/?q=";
    mapLink += String(latestGpsData.latitude, 6);
    mapLink += ",";
    mapLink += String(latestGpsData.longitude, 6);

    return mapLink;
}


// ================== Gui trang thai len Blynk ==================

void sendStatusToBlynk(const char *statusText, uint8_t alertStatus)
{
    String currentStatusText = String(statusText);

    if (lastSystemStatusSent != currentStatusText)
    {
        Blynk.virtualWrite(VPIN_SYSTEM_STATUS, currentStatusText);
        lastSystemStatusSent = currentStatusText;
    }

    if (lastAlertStatusSent != alertStatus)
    {
        Blynk.virtualWrite(VPIN_ALERT_STATUS, alertStatus);
        lastAlertStatusSent = alertStatus;
    }
}


// ================== Nhan du lieu tu Blynk ==================

BLYNK_WRITE(VPIN_SPEED_LIMIT)
{
    speedLimitKmh = (uint8_t)param.asInt();

    Serial.print("Speed limit updated: ");
    Serial.print(speedLimitKmh);
    Serial.println(" km/h");
}


BLYNK_WRITE(VPIN_RESET_MAX_SPEED)
{
    if (param.asInt() == 1)
    {
        maxSpeedKmh = 0U;

        Blynk.virtualWrite(VPIN_MAX_SPEED, maxSpeedKmh);
        Blynk.virtualWrite(VPIN_RESET_MAX_SPEED, 0);

        Serial.println("Max speed reset to 0 km/h.");
    }
}


BLYNK_WRITE(VPIN_GUARD_MODE)
{
    guardModeEnabled = (param.asInt() == 1);

    Serial.print("Guard mode: ");
    Serial.println(guardModeEnabled ? "ON" : "OFF");

    if (guardModeEnabled == false)
    {
        theftStrongAlarmActive = false;
        theftNotificationSent = false;
        lastHandledTamperCount = 0U;

        mpu6050ResetTheftState();
        updateLocalAlarmOutput();
    }
}


BLYNK_WRITE(VPIN_FIND_MOTORCYCLE)
{
    findMotorcycleEnabled = (param.asInt() == 1);

    Serial.print("Find motorcycle: ");
    Serial.println(findMotorcycleEnabled ? "ON" : "OFF");

    updateLocalAlarmOutput();
}


// ================== Doc GPS ==================

void taskReadGps(void)
{
    gpsUpdate();
    latestGpsData = gpsGetData();

    if (latestGpsData.isValid == true)
    {
        currentSpeedKmh = latestGpsData.speedKmh;

        /*
         * Cap nhat toc do cao nhat.
         * Dieu kien <= 150 de tranh GPS loi nhay toc do ao.
         */
        if ((currentSpeedKmh > maxSpeedKmh) && (currentSpeedKmh <= 150U))
        {
            maxSpeedKmh = currentSpeedKmh;
        }
    }
    else
    {
        currentSpeedKmh = 0U;
    }
}


// ================== Doc MPU6050 ==================

void taskReadMpu6050(void)
{
    mpu6050Update(guardModeEnabled);
}


// ================== Gui toc do len Blynk ==================

void taskUpdateBlynkSpeed(void)
{
    static uint8_t lastSpeedSent = 255U;
    static uint8_t lastMaxSpeedSent = 255U;

    if (currentSpeedKmh != lastSpeedSent)
    {
        Blynk.virtualWrite(VPIN_CURRENT_SPEED, currentSpeedKmh);
        lastSpeedSent = currentSpeedKmh;
    }

    if (maxSpeedKmh != lastMaxSpeedSent)
    {
        Blynk.virtualWrite(VPIN_MAX_SPEED, maxSpeedKmh);
        lastMaxSpeedSent = maxSpeedKmh;
    }
}


// ================== Gui vi tri len Blynk ==================

void taskUpdateBlynkLocation(void)
{
    if (latestGpsData.isValid == true)
    {
        String mapLink = buildMapLink();

        Blynk.virtualWrite(VPIN_LATITUDE, latestGpsData.latitude);
        Blynk.virtualWrite(VPIN_LONGITUDE, latestGpsData.longitude);
        Blynk.virtualWrite(VPIN_MAP_LINK, mapLink);
    }
    else
    {
        Blynk.virtualWrite(VPIN_LATITUDE, 0.0);
        Blynk.virtualWrite(VPIN_LONGITUDE, 0.0);
        Blynk.virtualWrite(VPIN_MAP_LINK, "Dang doi GPS...");
    }
}


// ================== Qua toc do ==================

void taskCheckOverspeed(void)
{
    unsigned long nowMs = millis();

    if (latestGpsData.isValid == false)
    {
        overspeedActive = false;
        wasOverspeed = false;

        updateLocalAlarmOutput();
        return;
    }

    if (currentSpeedKmh > speedLimitKmh)
    {
        overspeedActive = true;
        updateLocalAlarmOutput();

        if (wasOverspeed == false)
        {
            String message = "Xe vuot qua toc do. Toc do: ";
            message += String(currentSpeedKmh);
            message += " km/h. Gioi han: ";
            message += String(speedLimitKmh);
            message += " km/h.";

            Blynk.logEvent("overspeed_alert", message);

            lastOverspeedNotifyMs = nowMs;
            wasOverspeed = true;
        }
        else if ((nowMs - lastOverspeedNotifyMs) >= OVERSPEED_NOTIFY_INTERVAL_MS)
        {
            String message = "Xe van dang vuot qua toc do. Toc do: ";
            message += String(currentSpeedKmh);
            message += " km/h.";

            Blynk.logEvent("overspeed_alert", message);

            lastOverspeedNotifyMs = nowMs;
        }
    }
    else
    {
        overspeedActive = false;
        wasOverspeed = false;

        updateLocalAlarmOutput();
    }
}


// ================== Chong trom 3 cap ==================

void taskHandleTheftWarning(void)
{
    Mpu6050Data_t mpuData = mpu6050GetData();

    if (guardModeEnabled == false)
    {
        theftStrongAlarmActive = false;
        theftNotificationSent = false;
        lastHandledTamperCount = 0U;

        updateLocalAlarmOutput();
        return;
    }

    if (mpuData.isReady == false)
    {
        return;
    }

    /*
     * Neu MPU6050 reset ve muc 0 sau 30 giay khong tac dong,
     * cho phep he thong dem lai tu dau.
     */
    if (mpuData.tamperCount == 0U)
    {
        lastHandledTamperCount = 0U;
        theftNotificationSent = false;
    }

    /*
     * Chi xu ly khi so lan tac dong thay doi.
     */
    if ((mpuData.tamperCount > 0U) &&
        (mpuData.tamperCount != lastHandledTamperCount))
    {
        lastHandledTamperCount = mpuData.tamperCount;

        if (mpuData.theftLevel == THEFT_LEVEL_1)
        {
            Serial.println("Theft level 1 -> beep 1 time.");
            beepNTimes(1);
        }
        else if (mpuData.theftLevel == THEFT_LEVEL_2)
        {
            Serial.println("Theft level 2 -> beep 2 times.");
            beepNTimes(2);
        }
        else if (mpuData.theftLevel == THEFT_LEVEL_3)
        {
            Serial.println("Theft level 3 -> strong alarm and send theft_alert.");

            theftStrongAlarmActive = true;
            theftStrongAlarmStartMs = millis();

            updateLocalAlarmOutput();

            if (theftNotificationSent == false)
            {
                String message = "Canh bao nghi ngo trom. Vi tri: ";
                message += buildMapLink();

                Blynk.logEvent("theft_alert", message);
                theftNotificationSent = true;
            }
        }
    }

    /*
     * Canh bao muc 3 chi keu manh 5 giay.
     */
    if (theftStrongAlarmActive == true)
    {
        if ((millis() - theftStrongAlarmStartMs) >= THEFT_STRONG_ALARM_DURATION_MS)
        {
            theftStrongAlarmActive = false;
            updateLocalAlarmOutput();
        }
    }
}


// ================== Cap nhat V7/V8 ==================

void taskUpdateSystemStatus(void)
{
    Mpu6050Data_t mpuData = mpu6050GetData();

    /*
     * Thu tu uu tien:
     * 1. Nghi ngo trom
     * 2. Canh bao muc 2
     * 3. Canh bao muc 1
     * 4. Qua toc do
     * 5. Dang bao ve
     * 6. Dang doi GPS
     * 7. Binh thuong
     */
    if ((guardModeEnabled == true) && (mpuData.theftLevel == THEFT_LEVEL_3))
    {
        sendStatusToBlynk("Nghi ngo trom", 1U);
    }
    else if ((guardModeEnabled == true) && (mpuData.theftLevel == THEFT_LEVEL_2))
    {
        sendStatusToBlynk("Canh bao muc 2", 1U);
    }
    else if ((guardModeEnabled == true) && (mpuData.theftLevel == THEFT_LEVEL_1))
    {
        sendStatusToBlynk("Canh bao muc 1", 1U);
    }
    else if (overspeedActive == true)
    {
        sendStatusToBlynk("Qua toc do", 1U);
    }
    else if (guardModeEnabled == true)
    {
        sendStatusToBlynk("Dang bao ve", 0U);
    }
    else if (latestGpsData.isValid == false)
    {
        sendStatusToBlynk("Dang doi GPS", 0U);
    }
    else
    {
        sendStatusToBlynk("Binh thuong", 0U);
    }
}


// ================== Debug Serial ==================

void taskPrintDebug(void)
{
    Mpu6050Data_t mpuData = mpu6050GetData();

    Serial.print("GPS valid: ");
    Serial.print(latestGpsData.isValid ? "YES" : "NO");

    Serial.print(" | Speed: ");
    Serial.print(currentSpeedKmh);
    Serial.print(" km/h");

    Serial.print(" | Max Speed: ");
    Serial.print(maxSpeedKmh);
    Serial.print(" km/h");

    Serial.print(" | Lat: ");
    Serial.print(latestGpsData.latitude, 6);

    Serial.print(" | Lng: ");
    Serial.print(latestGpsData.longitude, 6);

    Serial.print(" | Satellites: ");
    Serial.print(latestGpsData.satellites);

    Serial.print(" | MPU ready: ");
    Serial.print(mpuData.isReady ? "YES" : "NO");

    Serial.print(" | Tamper: ");
    Serial.print(mpuData.tamperCount);

    Serial.print(" | AccelDelta: ");
    Serial.print(mpuData.accelDeltaG, 3);
    Serial.print(" g");

    Serial.print(" | Gyro: ");
    Serial.print(mpuData.gyroDps, 1);
    Serial.println(" deg/s");
}


// ================== Blynk connected ==================

BLYNK_CONNECTED()
{
    lastSystemStatusSent = "";
    lastAlertStatusSent = 255U;

    Blynk.syncVirtual(VPIN_SPEED_LIMIT);
    Blynk.syncVirtual(VPIN_GUARD_MODE);
    Blynk.syncVirtual(VPIN_FIND_MOTORCYCLE);

    Blynk.virtualWrite(VPIN_CURRENT_SPEED, currentSpeedKmh);
    Blynk.virtualWrite(VPIN_MAX_SPEED, maxSpeedKmh);

    Serial.println("Blynk connected and virtual pins synced.");
}


// ================== setup va loop ==================

void setup()
{
    Serial.begin(115200);
    delay(1000);

    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);

    buzzerOff();

    latestGpsData.isValid = false;
    latestGpsData.latitude = 0.0;
    latestGpsData.longitude = 0.0;
    latestGpsData.speedKmh = 0U;
    latestGpsData.satellites = 0U;

    Serial.println("ESP32 Motorcycle Monitor starting...");

    Serial.println("Initializing MPU6050...");
    mpu6050Init();

    Serial.println("Configuring GPS fast mode...");
    gpsInit();

    Serial.println("Connecting to WiFi and Blynk...");
    Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASSWORD);

    timer.setInterval(20L, taskReadGps);
    timer.setInterval(50L, taskReadMpu6050);

    /*
     * De tranh Blynk bi offline, gui toc do moi 500 ms.
     * Ham taskUpdateBlynkSpeed chi gui khi gia tri thay doi.
     */
    timer.setInterval(500L, taskUpdateBlynkSpeed);

    timer.setInterval(200L, taskCheckOverspeed);
    timer.setInterval(100L, taskHandleTheftWarning);
    timer.setInterval(500L, taskUpdateSystemStatus);

    /*
     * Vi tri va link map khong can gui nhanh.
     */
    timer.setInterval(30000L, taskUpdateBlynkLocation);

    /*
     * Debug Serial, khong gui len Blynk.
     */
    timer.setInterval(3000L, taskPrintDebug);
}


void loop()
{
    Blynk.run();
    timer.run();
}