#include "mpu6050_module.h"
#include "config.h"

#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <math.h>

/*
 * File: mpu6050_module.cpp
 * Chuc nang:
 * - Khoi tao MPU6050.
 * - Doc gia toc va gyro.
 * - Phat hien tac dong khi guard mode dang bat.
 * - Dem canh bao 3 cap:
 *   Lan 1 -> THEFT_LEVEL_1
 *   Lan 2 -> THEFT_LEVEL_2
 *   Lan 3 -> THEFT_LEVEL_3
 */

static Adafruit_MPU6050 mpu;

static Mpu6050Data_t mpuData = {
    false,             // isReady
    false,             // motionDetected
    THEFT_LEVEL_NONE,  // theftLevel
    0U,                // tamperCount
    0.0F,              // accelG
    0.0F,              // accelDeltaG
    0.0F               // gyroDps
};

/*
 * Nguong phat hien tac dong.
 * Neu qua nhay -> tang nguong.
 * Neu lac manh ma khong bao -> giam nguong.
 */
static const float ACCEL_DELTA_THRESHOLD_G = 0.18F;
static const float GYRO_THRESHOLD_DPS = 45.0F;

/*
 * Sau khi phat hien 1 lan tac dong, phai cho it nhat 2 giay
 * moi duoc dem lan tiep theo.
 */
static const unsigned long TAMPER_COOLDOWN_MS = 2000UL;

/*
 * Neu 30 giay khong co tac dong moi,
 * reset muc canh bao ve 0.
 */
static const unsigned long TAMPER_RESET_MS = 30000UL;

/*
 * Gia tri gia toc moc khi cam bien dang dung yen.
 */
static float baselineAccelG = 1.0F;

static unsigned long lastTamperTimeMs = 0UL;
static bool previousGuardModeEnabled = false;


/*
 * Doc gia toc tong va gyro tong.
 */
static bool mpu6050ReadRaw(float *accelMagnitudeG, float *gyroMagnitudeDps)
{
    if (mpuData.isReady == false)
    {
        return false;
    }

    sensors_event_t accelEvent;
    sensors_event_t gyroEvent;
    sensors_event_t tempEvent;

    mpu.getEvent(&accelEvent, &gyroEvent, &tempEvent);

    float ax = accelEvent.acceleration.x;
    float ay = accelEvent.acceleration.y;
    float az = accelEvent.acceleration.z;

    float gx = gyroEvent.gyro.x;
    float gy = gyroEvent.gyro.y;
    float gz = gyroEvent.gyro.z;

    /*
     * Tong gia toc doi sang don vi g.
     * Khi dung yen, gia tri thuong gan 1g.
     */
    *accelMagnitudeG = sqrt((ax * ax) + (ay * ay) + (az * az)) / 9.80665F;

    /*
     * Tong toc do quay doi tu rad/s sang deg/s.
     */
    *gyroMagnitudeDps = sqrt((gx * gx) + (gy * gy) + (gz * gz)) * 57.2958F;

    return true;
}


/*
 * Lay moc gia toc khi xe/module dang dung yen.
 */
static void mpu6050Calibrate(void)
{
    float sumAccel = 0.0F;
    const uint16_t sampleCount = 80U;

    for (uint16_t i = 0U; i < sampleCount; i++)
    {
        float accelG = 0.0F;
        float gyroDps = 0.0F;

        if (mpu6050ReadRaw(&accelG, &gyroDps) == true)
        {
            sumAccel += accelG;
        }

        delay(20);
    }

    baselineAccelG = sumAccel / (float)sampleCount;

    Serial.print("MPU6050 baseline accel = ");
    Serial.print(baselineAccelG, 3);
    Serial.println(" g");
}


void mpu6050Init(void)
{
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

    if (!mpu.begin(0x68, &Wire))
    {
        Serial.println("Khong tim thay MPU6050 tai dia chi 0x68.");
        Serial.println("Kiem tra day VCC, GND, SDA, SCL.");
        Serial.println("Neu module dung dia chi 0x69 thi can sua code.");
        mpuData.isReady = false;
        return;
    }

    mpuData.isReady = true;

    mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    Serial.println("Da tim thay MPU6050.");

    mpu6050Calibrate();
}


void mpu6050ResetTheftState(void)
{
    mpuData.motionDetected = false;
    mpuData.theftLevel = THEFT_LEVEL_NONE;
    mpuData.tamperCount = 0U;

    lastTamperTimeMs = 0UL;
}


void mpu6050Update(bool guardModeEnabled)
{
    if (mpuData.isReady == false)
    {
        return;
    }

    /*
     * Khi vua bat Guard Mode:
     * - reset so lan canh bao
     * - lay lai moc MPU6050 tai trang thai hien tai
     */
    if ((guardModeEnabled == true) && (previousGuardModeEnabled == false))
    {
        Serial.println("Guard mode ON -> reset MPU6050 theft state and calibrate.");
        mpu6050ResetTheftState();
        mpu6050Calibrate();
    }

    previousGuardModeEnabled = guardModeEnabled;

    /*
     * Guard Mode tat thi khong kiem tra chong trom.
     */
    if (guardModeEnabled == false)
    {
        mpu6050ResetTheftState();
        return;
    }

    float accelG = 0.0F;
    float gyroDps = 0.0F;

    if (mpu6050ReadRaw(&accelG, &gyroDps) == false)
    {
        return;
    }

    mpuData.accelG = accelG;
    mpuData.gyroDps = gyroDps;
    mpuData.accelDeltaG = fabs(accelG - baselineAccelG);
    mpuData.motionDetected = false;

    unsigned long nowMs = millis();

    /*
     * Neu lau khong co tac dong moi thi reset ve muc 0.
     */
    if ((mpuData.tamperCount > 0U) &&
        ((nowMs - lastTamperTimeMs) >= TAMPER_RESET_MS))
    {
        Serial.println("MPU6050: khong co tac dong trong 30 giay -> reset canh bao.");
        mpu6050ResetTheftState();
    }

    bool motionDetectedNow = false;

    if ((mpuData.accelDeltaG >= ACCEL_DELTA_THRESHOLD_G) ||
        (mpuData.gyroDps >= GYRO_THRESHOLD_DPS))
    {
        motionDetectedNow = true;
    }

    /*
     * Chi tinh la 1 lan tac dong moi neu da qua thoi gian cooldown.
     */
    if ((motionDetectedNow == true) &&
        ((nowMs - lastTamperTimeMs) >= TAMPER_COOLDOWN_MS))
    {
        mpuData.motionDetected = true;
        lastTamperTimeMs = nowMs;

        if (mpuData.tamperCount < 3U)
        {
            mpuData.tamperCount++;
        }

        if (mpuData.tamperCount == 1U)
        {
            mpuData.theftLevel = THEFT_LEVEL_1;
        }
        else if (mpuData.tamperCount == 2U)
        {
            mpuData.theftLevel = THEFT_LEVEL_2;
        }
        else
        {
            mpuData.theftLevel = THEFT_LEVEL_3;
        }

        Serial.print("MPU6050: phat hien tac dong lan ");
        Serial.print(mpuData.tamperCount);
        Serial.print(" | Accel delta = ");
        Serial.print(mpuData.accelDeltaG, 3);
        Serial.print(" g | Gyro = ");
        Serial.print(mpuData.gyroDps, 1);
        Serial.println(" deg/s");
    }
}


Mpu6050Data_t mpu6050GetData(void)
{
    return mpuData;
}