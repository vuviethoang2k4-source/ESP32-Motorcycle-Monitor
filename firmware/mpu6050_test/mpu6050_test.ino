/*
 * File: mpu6050_test.ino
 * Chuc nang:
 * - Test MPU6050 voi ESP32 DevKit V1.
 * - Doc do rung/lac bang gia toc va gyro.
 * - Test logic chong trom:
 *   Lan 1: coi bip 1 lan
 *   Lan 2: coi bip 2 lan
 *   Lan 3: coi keu manh va in canh bao
 */

#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <math.h>

// ================== Pin Configuration ==================

#define I2C_SDA_PIN     21
#define I2C_SCL_PIN     22

// Coi active HIGH da test: HIGH = keu, LOW = tat
#define BUZZER_PIN      25


// ================== MPU6050 Object ==================

Adafruit_MPU6050 mpu;


// ================== Threshold Configuration ==================

/*
 * Nguong phat hien rung/lac.
 * Neu nhay qua nhieu -> tang nguong.
 * Neu lac manh ma khong bao -> giam nguong.
 */
const float ACCEL_DELTA_THRESHOLD_G = 0.18;     // do lech gia toc so voi luc dung yen
const float GYRO_THRESHOLD_DPS      = 45.0;     // toc do quay, don vi deg/s

/*
 * Sau khi phat hien 1 lan tac dong, cho it nhat 2 giay
 * moi cho dem lan tiep theo.
 */
const unsigned long TAMPER_COOLDOWN_MS = 2000UL;

/*
 * Neu qua 30 giay khong bi tac dong tiep,
 * reset so lan canh bao ve 0.
 */
const unsigned long TAMPER_RESET_MS = 30000UL;


// ================== State Variables ==================

float baselineAccelG = 1.0;

uint8_t tamperCount = 0;

unsigned long lastTamperTimeMs = 0;
unsigned long lastPrintTimeMs = 0;


// ================== Buzzer Functions ==================

void buzzerOff()
{
    digitalWrite(BUZZER_PIN, LOW);
}


void buzzerOn()
{
    digitalWrite(BUZZER_PIN, HIGH);
}


void beepOnce()
{
    buzzerOn();
    delay(120);
    buzzerOff();
}


void beepNTimes(uint8_t times)
{
    for (uint8_t i = 0; i < times; i++)
    {
        beepOnce();
        delay(150);
    }
}


void strongAlarm()
{
    /*
     * Canh bao manh: coi keu theo chu ky nhanh trong 5 giay.
     */
    unsigned long startMs = millis();

    while ((millis() - startMs) < 5000UL)
    {
        buzzerOn();
        delay(200);
        buzzerOff();
        delay(120);
    }
}


// ================== MPU Read Function ==================

bool readMpuData(float *accelMagnitudeG, float *gyroMagnitudeDps)
{
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
     * Tong gia toc, doi sang don vi g.
     * Khi dung yen, gia tri thuong gan 1g.
     */
    *accelMagnitudeG = sqrt((ax * ax) + (ay * ay) + (az * az)) / 9.80665;

    /*
     * Tong toc do quay, doi tu rad/s sang deg/s.
     */
    *gyroMagnitudeDps = sqrt((gx * gx) + (gy * gy) + (gz * gz)) * 57.2958;

    return true;
}


// ================== Calibration ==================

void calibrateMpu6050()
{
    Serial.println("Dang lay moc MPU6050, de cam bien dung yen 3 giay...");

    float sumAccel = 0.0;
    const uint16_t sampleCount = 100;

    for (uint16_t i = 0; i < sampleCount; i++)
    {
        float accelG = 0.0;
        float gyroDps = 0.0;

        readMpuData(&accelG, &gyroDps);
        sumAccel += accelG;

        delay(30);
    }

    baselineAccelG = sumAccel / sampleCount;

    Serial.print("Baseline Accel = ");
    Serial.print(baselineAccelG, 3);
    Serial.println(" g");
}


// ================== Setup ==================

void setup()
{
    Serial.begin(115200);
    delay(1000);

    pinMode(BUZZER_PIN, OUTPUT);
    buzzerOff();

    Serial.println();
    Serial.println("=================================");
    Serial.println("MPU6050 Theft Logic Test");
    Serial.println("ESP32 DevKit V1");
    Serial.println("=================================");

    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

    if (!mpu.begin(0x68, &Wire))
    {
        Serial.println("Khong tim thay MPU6050 tai dia chi 0x68.");
        Serial.println("Thu doi sang 0x69 trong code neu module cua ban dung dia chi 0x69.");
        Serial.println("Kiem tra day: VCC-3V3, GND-GND, SDA-GPIO21, SCL-GPIO22.");

        while (1)
        {
            delay(1000);
        }
    }

    Serial.println("Da tim thay MPU6050.");

    mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    calibrateMpu6050();

    Serial.println("San sang test.");
    Serial.println("Lan 1: bip 1 lan");
    Serial.println("Lan 2: bip 2 lan");
    Serial.println("Lan 3: coi keu manh");
    Serial.println();
}


// ================== Loop ==================

void loop()
{
    float accelG = 0.0;
    float gyroDps = 0.0;

    readMpuData(&accelG, &gyroDps);

    float accelDeltaG = fabs(accelG - baselineAccelG);

    unsigned long nowMs = millis();

    bool motionDetected = false;

    if ((accelDeltaG >= ACCEL_DELTA_THRESHOLD_G) ||
        (gyroDps >= GYRO_THRESHOLD_DPS))
    {
        motionDetected = true;
    }

    /*
     * In du lieu moi 500 ms de quan sat nguong.
     */
    if ((nowMs - lastPrintTimeMs) >= 500UL)
    {
        lastPrintTimeMs = nowMs;

        Serial.print("Accel=");
        Serial.print(accelG, 3);
        Serial.print(" g");

        Serial.print(" | Delta=");
        Serial.print(accelDeltaG, 3);
        Serial.print(" g");

        Serial.print(" | Gyro=");
        Serial.print(gyroDps, 1);
        Serial.print(" deg/s");

        Serial.print(" | Tamper count=");
        Serial.println(tamperCount);
    }

    /*
     * Neu qua lau khong co tac dong moi thi reset so lan.
     */
    if ((tamperCount > 0) &&
        ((nowMs - lastTamperTimeMs) >= TAMPER_RESET_MS))
    {
        tamperCount = 0;
        Serial.println("Khong co tac dong trong 30 giay -> reset tamperCount ve 0.");
    }

    /*
     * Neu phat hien tac dong va da qua cooldown thi tinh la 1 lan moi.
     */
    if ((motionDetected == true) &&
        ((nowMs - lastTamperTimeMs) >= TAMPER_COOLDOWN_MS))
    {
        lastTamperTimeMs = nowMs;

        if (tamperCount < 3)
        {
            tamperCount++;
        }

        Serial.println();
        Serial.print(">>> PHAT HIEN TAC DONG LAN ");
        Serial.println(tamperCount);

        if (tamperCount == 1)
        {
            Serial.println("Canh bao muc 1: bip 1 lan.");
            beepNTimes(1);
        }
        else if (tamperCount == 2)
        {
            Serial.println("Canh bao muc 2: bip 2 lan.");
            beepNTimes(2);
        }
        else
        {
            Serial.println("Canh bao muc 3: COI KEU MANH. Sau nay se gui theft_alert len Blynk.");
            strongAlarm();
        }

        Serial.println();
    }

    delay(50);
}