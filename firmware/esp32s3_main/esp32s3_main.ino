/*
 * File: esp32s3_main.ino
 * Chuc nang:
 * - Ket noi ESP32-S3 voi WiFi va Blynk.
 * - Test Blynk App bang du lieu gia.
 * - Kiem tra cac Virtual Pin V0 den V8.
 * - Chua doc GPS NEO-8M va MPU6050 that o buoc nay.
 */

#include "secrets.h"   // Chua WiFi va Blynk token that, khong push len GitHub
#include "config.h"    // Chua Virtual Pin va cac hang so cau hinh

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// Timer cua Blynk, dung de chay cac tac vu dinh ky thay cho delay()
BlynkTimer timer;


// ================== Bien trang thai he thong ==================

// Toc do hien tai, don vi km/h
uint8_t currentSpeedKmh = 0U;

// Toc do gioi han, mac dinh lay tu config.h
uint8_t speedLimitKmh = DEFAULT_SPEED_LIMIT_KMH;

// Trang thai bat/tat che do bao ve xe
bool guardModeEnabled = false;

// Trang thai bat/tat chuc nang tim xe
bool findMotorcycleEnabled = false;

// Co ghi nho lan truoc xe co qua toc do hay khong
bool wasOverspeed = false;

// Thoi diem gui thong bao qua toc do lan gan nhat
unsigned long lastOverspeedNotifyMs = 0UL;


// ================== Du lieu GPS gia de test Blynk ==================

// Vi do gia, gan khu vuc Ha Noi
double fakeLatitude = 21.028511;

// Kinh do gia, gan khu vuc Ha Noi
double fakeLongitude = 105.804817;


// ================== Ham nhan du lieu tu Blynk App ==================

/*
 * V1 - Speed Limit
 * Nguoi dung thay doi toc do toi da tren app.
 */
BLYNK_WRITE(VPIN_SPEED_LIMIT)
{
    speedLimitKmh = (uint8_t)param.asInt();
}

/*
 * V2 - Guard Mode
 * Nguoi dung bat/tat che do bao ve xe.
 */
BLYNK_WRITE(VPIN_GUARD_MODE)
{
    guardModeEnabled = (param.asInt() == 1);
}

/*
 * V3 - Find Motorcycle
 * Nguoi dung bat/tat chuc nang tim xe.
 */
BLYNK_WRITE(VPIN_FIND_MOTORCYCLE)
{
    findMotorcycleEnabled = (param.asInt() == 1);

    if (findMotorcycleEnabled)
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


// ================== Ham gui du lieu gia len Blynk ==================

/*
 * Ham nay tao toc do gia de test app.
 * Toc do se tang dan tu 0 den 90 km/h, sau do quay lai 0.
 */
void taskGenerateFakeSpeed(void)
{
    currentSpeedKmh += 5U;

    if (currentSpeedKmh > 90U)
    {
        currentSpeedKmh = 0U;
    }
}


/*
 * Ham nay gui toc do, vi tri va link Google Maps len Blynk App.
 */
void taskUpdateBlynkData(void)
{
    String mapLink = "https://maps.google.com/?q=";
    mapLink += String(fakeLatitude, 6);
    mapLink += ",";
    mapLink += String(fakeLongitude, 6);

    Blynk.virtualWrite(VPIN_CURRENT_SPEED, currentSpeedKmh);
    Blynk.virtualWrite(VPIN_LATITUDE, fakeLatitude);
    Blynk.virtualWrite(VPIN_LONGITUDE, fakeLongitude);
    Blynk.virtualWrite(VPIN_MAP_LINK, mapLink);
}


/*
 * Ham nay kiem tra qua toc do.
 * Neu toc do hien tai lon hon toc do gioi han thi:
 * - V7 hien thi "Qua toc do"
 * - V8 bat trang thai canh bao
 * - Gui event overspeed_alert
 */
void taskCheckOverspeed(void)
{
    unsigned long nowMs = millis();

    if (currentSpeedKmh > speedLimitKmh)
    {
        Blynk.virtualWrite(VPIN_SYSTEM_STATUS, "Qua toc do");
        Blynk.virtualWrite(VPIN_ALERT_STATUS, 1);

        if (!wasOverspeed)
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
        Blynk.virtualWrite(VPIN_SYSTEM_STATUS, "Binh thuong");
        Blynk.virtualWrite(VPIN_ALERT_STATUS, 0);
        wasOverspeed = false;
    }
}


/*
 * Ham nay dong bo du lieu tu app xuong ESP32 khi vua ket noi Blynk.
 */
BLYNK_CONNECTED()
{
    Blynk.syncVirtual(VPIN_SPEED_LIMIT);
    Blynk.syncVirtual(VPIN_GUARD_MODE);
    Blynk.syncVirtual(VPIN_FIND_MOTORCYCLE);
}


// ================== setup va loop ==================

void setup()
{
    Serial.begin(115200);

    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);

    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);

    Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASSWORD);

    timer.setInterval(1000L, taskGenerateFakeSpeed);
    timer.setInterval(1000L, taskUpdateBlynkData);
    timer.setInterval(1000L, taskCheckOverspeed);
}


void loop()
{
    Blynk.run();
    timer.run();
}