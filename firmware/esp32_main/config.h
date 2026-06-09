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
#define VPIN_MAX_SPEED          V9
#define VPIN_RESET_MAX_SPEED    V10


// ================== Default Values ==================

// Toc do gioi han mac dinh, don vi km/h
static const uint8_t DEFAULT_SPEED_LIMIT_KMH = 50U;

// Khoang thoi gian gui lai canh bao qua toc do, don vi ms
static const unsigned long OVERSPEED_NOTIFY_INTERVAL_MS = 15000UL;

// Khoang thoi gian gui du lieu len Blynk, don vi ms
static const unsigned long BLYNK_UPDATE_INTERVAL_MS = 1000UL;


// ================== Hardware Pins ==================

// Chan dieu khien buzzer va LED tren ESP32 DevKit V1
// Khong dung GPIO10/GPIO11 vi cac chan nay lien quan den Flash
#define BUZZER_PIN              25
#define LED_PIN                 26


// ================== GPS UART Pins ==================

// GPS NEO-8M giao tiep voi ESP32 DevKit V1 bang UART2
// GPS TX -> ESP32 GPIO16
// GPS RX -> ESP32 GPIO17
#define GPS_RX_PIN              16
#define GPS_TX_PIN              17

// Baudrate mac dinh cua GPS NEO-8M
#define GPS_BAUD_RATE           9600


// ================== MPU6050 I2C Pins ==================

// MPU6050 giao tiep voi ESP32 DevKit V1 bang I2C
#define I2C_SDA_PIN             21
#define I2C_SCL_PIN             22
#endif