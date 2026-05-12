#ifndef CONFIG_H
#define CONFIG_H

/*
 * File: config.h
 * Chuc nang:
 * - Khai bao Virtual Pin dung voi Blynk App.
 * - Khai bao cac hang so cau hinh cho chuong trinh.
 * - Khong chua WiFi password hoac Blynk token.
 */

// ================== Blynk Virtual Pins ==================

// V0: Toc do hien tai cua xe, don vi km/h
#define VPIN_CURRENT_SPEED      V0

// V1: Toc do toi da do nguoi dung cai tren app
#define VPIN_SPEED_LIMIT        V1

// V2: Bat/tat che do bao ve xe
#define VPIN_GUARD_MODE         V2

// V3: Lenh tim xe tu app
#define VPIN_FIND_MOTORCYCLE    V3

// V4: Vi do GPS
#define VPIN_LATITUDE           V4

// V5: Kinh do GPS
#define VPIN_LONGITUDE          V5

// V6: Link Google Maps
#define VPIN_MAP_LINK           V6

// V7: Trang thai dang chu
#define VPIN_SYSTEM_STATUS      V7

// V8: Trang thai canh bao dang mau/den
#define VPIN_ALERT_STATUS       V8


// ================== Default Values ==================

// Toc do gioi han mac dinh, don vi km/h
static const uint8_t DEFAULT_SPEED_LIMIT_KMH = 50U;

// Khoang thoi gian gui lai canh bao qua toc do, don vi ms
static const unsigned long OVERSPEED_NOTIFY_INTERVAL_MS = 15000UL;

// Khoang thoi gian gui du lieu len Blynk, don vi ms
static const unsigned long BLYNK_UPDATE_INTERVAL_MS = 1000UL;


// ================== Hardware Pins ==================

// Tam thoi chua chot chan that, se cap nhat sau khi noi mach
#define BUZZER_PIN              10
#define LED_PIN                 11

#endif