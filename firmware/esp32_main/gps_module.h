#ifndef GPS_MODULE_H
#define GPS_MODULE_H

/*
 * File: gps_module.h
 * Chuc nang:
 * - Khai bao du lieu GPS.
 * - Khai bao cac ham khoi tao va doc GPS NEO-8M.
 */

#include <Arduino.h>

// Cau truc luu du lieu GPS can dung trong he thong
typedef struct
{
    bool isValid;          // true neu GPS co du lieu hop le
    double latitude;       // vi do
    double longitude;      // kinh do
    uint8_t speedKmh;      // toc do, don vi km/h
    uint8_t satellites;    // so ve tinh GPS
} GpsData_t;

// Khoi tao UART cho GPS
void gpsInit(void);

// Doc va cap nhat du lieu GPS moi nhat
void gpsUpdate(void);

// Lay du lieu GPS hien tai
GpsData_t gpsGetData(void);

#endif