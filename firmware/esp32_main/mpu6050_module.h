#ifndef MPU6050_MODULE_H
#define MPU6050_MODULE_H

/*
 * File: mpu6050_module.h
 * Chuc nang: khai bao module MPU6050 dung cho chong trom 3 cap.
 */

#include <Arduino.h>

typedef enum
{
    THEFT_LEVEL_NONE = 0,
    THEFT_LEVEL_1,
    THEFT_LEVEL_2,
    THEFT_LEVEL_3
} TheftLevel_t;

typedef struct
{
    bool isReady;
    bool motionDetected;
    TheftLevel_t theftLevel;
    uint8_t tamperCount;
    float accelG;
    float accelDeltaG;
    float gyroDps;
} Mpu6050Data_t;

// Khoi tao MPU6050
void mpu6050Init(void);

// Doc MPU6050 va cap nhat logic phat hien tac dong
void mpu6050Update(bool guardModeEnabled);

// Lay du lieu MPU6050 hien tai
Mpu6050Data_t mpu6050GetData(void);

// Reset trang thai canh bao chong trom
void mpu6050ResetTheftState(void);

#endif