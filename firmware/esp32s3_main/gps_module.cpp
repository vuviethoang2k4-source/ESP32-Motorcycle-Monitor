#include "gps_module.h"
#include "config.h"

#include <TinyGPSPlus.h>

/*
 * File: gps_module.cpp
 * Chuc nang:
 * - Khoi tao UART2 de doc GPS NEO-8M.
 * - Cau hinh GPS sang che do nhanh hon:
 *   + UART GPS chay 38400 baud.
 *   + GPS cap nhat 5Hz, tuc 200 ms/mau.
 *   + Giam bot cau NMEA khong can thiet de tranh nghen UART.
 * - Doc toc do, vi do, kinh do, so ve tinh.
 */

static TinyGPSPlus gpsParser;
static HardwareSerial gpsSerial(2);

static const uint32_t GPS_INITIAL_BAUD_RATE = 9600UL;
static const uint32_t GPS_FAST_BAUD_RATE = 38400UL;
static const uint32_t GPS_DATA_TIMEOUT_MS = 2000UL;

static GpsData_t gpsData = {
    false,
    0.0,
    0.0,
    0U,
    0U
};


/*
 * Gui mot goi lenh UBX toi GPS.
 */
static void gpsSendUbxCommand(const uint8_t *command, uint8_t length)
{
    gpsSerial.write(command, length);
    gpsSerial.flush();
    delay(80);
}


/*
 * Cau hinh GPS:
 * - Doi baudrate tu 9600 len 38400.
 * - Chi giu lai GGA va RMC.
 * - Tat GLL, GSA, GSV, VTG de giam luong du lieu.
 * - Dat update rate = 5Hz.
 */
static void gpsConfigureFastMode(void)
{
    /*
     * UBX-CFG-PRT:
     * Doi UART1 cua GPS sang 38400 baud, 8N1.
     */
    static const uint8_t setBaud38400[] = {
        0xB5, 0x62,
        0x06, 0x00,
        0x14, 0x00,
        0x01, 0x00,
        0x00, 0x00,
        0xD0, 0x08, 0x00, 0x00,
        0x00, 0x96, 0x00, 0x00,
        0x07, 0x00,
        0x03, 0x00,
        0x00, 0x00,
        0x00, 0x00,
        0x93, 0x90
    };

    /*
     * UBX-CFG-MSG:
     * Enable GGA tren UART1.
     */
    static const uint8_t enableGga[] = {
        0xB5, 0x62,
        0x06, 0x01,
        0x08, 0x00,
        0xF0, 0x00,
        0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x28
    };

    /*
     * Enable RMC tren UART1.
     * RMC co toc do GPS nen can giu lai.
     */
    static const uint8_t enableRmc[] = {
        0xB5, 0x62,
        0x06, 0x01,
        0x08, 0x00,
        0xF0, 0x04,
        0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
        0x04, 0x44
    };

    /*
     * Disable GLL.
     */
    static const uint8_t disableGll[] = {
        0xB5, 0x62,
        0x06, 0x01,
        0x08, 0x00,
        0xF0, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x2A
    };

    /*
     * Disable GSA.
     */
    static const uint8_t disableGsa[] = {
        0xB5, 0x62,
        0x06, 0x01,
        0x08, 0x00,
        0xF0, 0x02,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x01, 0x31
    };

    /*
     * Disable GSV.
     * GSV la cau ve thong tin ve tinh, rat dai, de gay cham UART.
     */
    static const uint8_t disableGsv[] = {
        0xB5, 0x62,
        0x06, 0x01,
        0x08, 0x00,
        0xF0, 0x03,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x02, 0x38
    };

    /*
     * Disable VTG.
     * Toc do da lay tu RMC nen co the tat VTG.
     */
    static const uint8_t disableVtg[] = {
        0xB5, 0x62,
        0x06, 0x01,
        0x08, 0x00,
        0xF0, 0x05,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x04, 0x46
    };

    /*
     * UBX-CFG-RATE:
     * Measurement rate = 200 ms
     * Navigation rate = 1
     * Time reference = GPS time
     * => 5Hz
     */
    static const uint8_t setRate5Hz[] = {
        0xB5, 0x62,
        0x06, 0x08,
        0x06, 0x00,
        0xC8, 0x00,
        0x01, 0x00,
        0x01, 0x00,
        0xDE, 0x6A
    };

    /*
     * Buoc 1:
     * GPS thuong mac dinh la 9600 baud.
     * Gui lenh doi baudrate len 38400.
     */
    gpsSendUbxCommand(setBaud38400, sizeof(setBaud38400));
    delay(200);

    /*
     * Buoc 2:
     * ESP32 doi UART2 sang 38400 de noi chuyen tiep voi GPS.
     */
    gpsSerial.end();
    delay(100);
    gpsSerial.begin(GPS_FAST_BAUD_RATE, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
    delay(200);

    /*
     * Buoc 3:
     * Giam bot cau NMEA va tang toc cap nhat GPS.
     */
    gpsSendUbxCommand(enableGga, sizeof(enableGga));
    gpsSendUbxCommand(enableRmc, sizeof(enableRmc));

    gpsSendUbxCommand(disableGll, sizeof(disableGll));
    gpsSendUbxCommand(disableGsa, sizeof(disableGsa));
    gpsSendUbxCommand(disableGsv, sizeof(disableGsv));
    gpsSendUbxCommand(disableVtg, sizeof(disableVtg));

    gpsSendUbxCommand(setRate5Hz, sizeof(setRate5Hz));
}


void gpsInit(void)
{
    /*
     * Ban dau mo UART o 9600 de gui lenh cau hinh cho GPS.
     */
    gpsSerial.begin(GPS_INITIAL_BAUD_RATE, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
    delay(500);

    gpsConfigureFastMode();
}


void gpsUpdate(void)
{
    while (gpsSerial.available() > 0)
    {
        char gpsChar = (char)gpsSerial.read();
        gpsParser.encode(gpsChar);
    }

    bool locationFresh = false;

    if ((gpsParser.location.isValid() == true) &&
        (gpsParser.location.age() < GPS_DATA_TIMEOUT_MS))
    {
        locationFresh = true;
    }

    if (locationFresh == true)
    {
        gpsData.isValid = true;
        gpsData.latitude = gpsParser.location.lat();
        gpsData.longitude = gpsParser.location.lng();

        if ((gpsParser.speed.isValid() == true) &&
            (gpsParser.speed.age() < GPS_DATA_TIMEOUT_MS))
        {
            /*
             * Lam tron toc do de hien thi on dinh hon tren app.
             */
            gpsData.speedKmh = (uint8_t)(gpsParser.speed.kmph() + 0.5);
        }
        else
        {
            gpsData.speedKmh = 0U;
        }

        if ((gpsParser.satellites.isValid() == true) &&
            (gpsParser.satellites.age() < GPS_DATA_TIMEOUT_MS))
        {
            gpsData.satellites = (uint8_t)gpsParser.satellites.value();
        }
    }
    else
    {
        gpsData.isValid = false;
        gpsData.speedKmh = 0U;
    }
}


GpsData_t gpsGetData(void)
{
    return gpsData;
}