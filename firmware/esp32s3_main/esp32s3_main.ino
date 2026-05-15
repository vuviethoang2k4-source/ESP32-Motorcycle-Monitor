/*
 * File: esp32s3_main.ino
 * Chuc nang:
 * - Ket noi ESP32 DevKit V1 voi WiFi va Blynk.
 * - Doc GPS NEO-8M o che do nhanh hon.
 * - Gui toc do len Blynk nhanh hon.
 * - Tach viec gui toc do va gui vi tri de tranh nghen Blynk.
 * - Canh bao qua toc do bang Blynk notification va coi buzzer.
 * - Nhan lenh Bao ve xe va Tim xe tu app.
 * - Chua xu ly MPU6050 trong buoc nay.
 */

#include "secrets.h"
#include "config.h"
#include "gps_module.h"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

BlynkTimer timer;


// ================== Bien trang thai he thong ==================

uint8_t currentSpeedKmh = 0U;

uint8_t speedLimitKmh = DEFAULT_SPEED_LIMIT_KMH;

bool guardModeEnabled = false;

bool findMotorcycleEnabled = false;

bool overspeedActive = false;

bool wasOverspeed = false;

unsigned long lastOverspeedNotifyMs = 0UL;

GpsData_t latestGpsData;


/*
 * Bien nho trang thai da gui len Blynk.
 * Muc dich:
 * - Khong gui lap lai "Binh thuong" / "Qua toc do" qua nhieu lan.
 * - Giam do tre do Blynk bi spam.
 */
String lastSystemStatusSent = "";
uint8_t lastAlertStatusSent = 255U;


// ================== Dieu khien coi / LED ==================

void updateLocalAlarmOutput(void)
{
    bool alarmOutputOn = false;

    if ((overspeedActive == true) || (findMotorcycleEnabled == true))
    {
        alarmOutputOn = true;
    }

    if (alarmOutputOn == true)
    {
        digitalWrite(BUZZER_PIN, HIGH);
        digitalWrite(LED_PIN, HIGH);
    }
    else
    {
        digitalWrite(BUZZER_PIN, LOW);
        digitalWrite(LED_PIN, LOW);
    }
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


// ================== Nhan du lieu tu Blynk App ==================

BLYNK_WRITE(VPIN_SPEED_LIMIT)
{
    speedLimitKmh = (uint8_t)param.asInt();

    Serial.print("Speed limit updated: ");
    Serial.print(speedLimitKmh);
    Serial.println(" km/h");
}


BLYNK_WRITE(VPIN_GUARD_MODE)
{
    guardModeEnabled = (param.asInt() == 1);

    Serial.print("Guard mode: ");
    Serial.println(guardModeEnabled ? "ON" : "OFF");
}


BLYNK_WRITE(VPIN_FIND_MOTORCYCLE)
{
    findMotorcycleEnabled = (param.asInt() == 1);

    Serial.print("Find motorcycle: ");
    Serial.println(findMotorcycleEnabled ? "ON" : "OFF");

    updateLocalAlarmOutput();
}


// ================== Tac vu GPS ==================

void taskReadGps(void)
{
    gpsUpdate();
    latestGpsData = gpsGetData();

    if (latestGpsData.isValid == true)
    {
        currentSpeedKmh = latestGpsData.speedKmh;
    }
    else
    {
        currentSpeedKmh = 0U;
    }
}


void taskPrintGpsDebug(void)
{
    Serial.print("GPS valid: ");
    Serial.print(latestGpsData.isValid ? "YES" : "NO");

    Serial.print(" | Speed: ");
    Serial.print(currentSpeedKmh);
    Serial.print(" km/h");

    Serial.print(" | Lat: ");
    Serial.print(latestGpsData.latitude, 6);

    Serial.print(" | Lng: ");
    Serial.print(latestGpsData.longitude, 6);

    Serial.print(" | Satellites: ");
    Serial.println(latestGpsData.satellites);
}


// ================== Gui du lieu len Blynk ==================

/*
 * Gui rieng toc do len Blynk.
 * Ham nay chay nhanh hon cac du lieu khac.
 */
void taskUpdateBlynkSpeed(void)
{
    Blynk.virtualWrite(VPIN_CURRENT_SPEED, currentSpeedKmh);
}


/*
 * Gui vi tri cham hon toc do.
 * Vi tri va map link la chuoi dai, gui qua nhanh se lam Blynk cham.
 */
void taskUpdateBlynkLocation(void)
{
    if (latestGpsData.isValid == true)
    {
        String mapLink = "https://maps.google.com/?q=";
        mapLink += String(latestGpsData.latitude, 6);
        mapLink += ",";
        mapLink += String(latestGpsData.longitude, 6);

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


/*
 * Kiem tra qua toc do.
 *
 * Toi uu:
 * - Coi duoc bat/tat tai ESP32, khong phu thuoc delay cua Blynk.
 * - Trang thai Blynk chi gui khi co thay doi.
 */
void taskCheckOverspeed(void)
{
    unsigned long nowMs = millis();

    if (latestGpsData.isValid == false)
    {
        overspeedActive = false;
        wasOverspeed = false;

        updateLocalAlarmOutput();
        sendStatusToBlynk("Dang doi GPS", 0U);
        return;
    }

    if (currentSpeedKmh > speedLimitKmh)
    {
        overspeedActive = true;

        updateLocalAlarmOutput();
        sendStatusToBlynk("Qua toc do", 1U);

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
        sendStatusToBlynk("Binh thuong", 0U);
    }
}


BLYNK_CONNECTED()
{
    /*
     * Reset bo nho trang thai de khi ket noi lai Blynk,
     * ESP32 se gui lai trang thai hien tai len app.
     */
    lastSystemStatusSent = "";
    lastAlertStatusSent = 255U;

    Blynk.syncVirtual(VPIN_SPEED_LIMIT);
    Blynk.syncVirtual(VPIN_GUARD_MODE);
    Blynk.syncVirtual(VPIN_FIND_MOTORCYCLE);

    Serial.println("Blynk connected and virtual pins synced.");
}


// ================== setup va loop ==================

void setup()
{
    Serial.begin(115200);
    delay(1000);

    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);

    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);

    latestGpsData.isValid = false;
    latestGpsData.latitude = 0.0;
    latestGpsData.longitude = 0.0;
    latestGpsData.speedKmh = 0U;
    latestGpsData.satellites = 0U;

    Serial.println("ESP32 Motorcycle Monitor starting...");
    Serial.println("Configuring GPS fast mode...");
    gpsInit();

    Serial.println("Connecting to WiFi and Blynk...");
    Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASSWORD);

    /*
     * Doc GPS moi 20 ms.
     * GPS da cau hinh 5Hz, tuc co du lieu moi khoang 200 ms.
     * Doc UART nhanh de khong mat byte.
     */
    timer.setInterval(20L, taskReadGps);

    /*
     * Gui toc do moi 250 ms.
     * Khong nen thap hon nua vi Blynk Cloud co gioi han toc do gui.
     */
    timer.setInterval(250L, taskUpdateBlynkSpeed);

    /*
     * Kiem tra qua toc do moi 200 ms.
     * Coi se phan ung nhanh hon app.
     */
    timer.setInterval(200L, taskCheckOverspeed);

    /*
     * Gui vi tri moi 2 giay.
     * Vi tri va link map khong can nhanh nhu toc do.
     */
    timer.setInterval(30000L, taskUpdateBlynkLocation);

    /*
     * In debug moi 2 giay.
     */
    timer.setInterval(2000L, taskPrintGpsDebug);
}


void loop()
{
    Blynk.run();
    timer.run();
}