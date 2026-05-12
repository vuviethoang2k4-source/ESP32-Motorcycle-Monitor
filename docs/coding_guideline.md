\# Coding Guideline



This project follows a simplified embedded C/C++ coding guideline based on MISRA-C principles and Ganssle firmware development practices.



&#x20;1. Naming Rules



Use meaningful names for variables, functions, and constants.



\### Variables and functions



Use lowerCamelCase.



Examples:



```cpp

uint8\_t currentSpeedKmh;

uint8\_t speedLimitKmh;

bool guardModeEnabled;

bool theftDetected;

void updateBlynkData(void);



Constants and macros



Use UPPER\_CASE.



Examples:



\#define GPS\_RX\_PIN 16U

\#define GPS\_TX\_PIN 17U



static const uint8\_t DEFAULT\_SPEED\_LIMIT\_KMH = 50U;

2\. No Magic Numbers



Do not use unexplained numbers directly in the code.



Bad example:



if (currentSpeedKmh > 50)

{

&#x20;   // Overspeed

}



Good example:



static const uint8\_t DEFAULT\_SPEED\_LIMIT\_KMH = 50U;



if (currentSpeedKmh > speedLimitKmh)

{

&#x20;   // Overspeed

}

3\. Modular Firmware Design



The firmware shall be divided into separate modules.



Module	Responsibility

config.h	Pin definitions and configuration values

blynk\_module	Blynk communication

gps\_module	Read GPS speed and location

mpu6050\_module	Read vibration and motion data

alarm\_logic	Process overspeed and theft warnings

4\. Avoid Blocking Delay



The main program should avoid using delay() for periodic tasks.



Bad example:



delay(1000);



Good example:



BlynkTimer timer;



timer.setInterval(1000L, taskUpdateBlynk);

timer.setInterval(200L, taskReadMpu6050);

timer.setInterval(500L, taskCheckAlarm);

5\. Main Loop Rule



The loop() function should be short and clear.



void loop()

{

&#x20;   Blynk.run();

&#x20;   timer.run();

}



Sensor reading, warning logic, and Blynk updating should be placed in separate functions.



6\. System Status



System status should be represented by enum values.



typedef enum

{

&#x20;   SYSTEM\_STATUS\_NORMAL = 0,

&#x20;   SYSTEM\_STATUS\_OVERSPEED,

&#x20;   SYSTEM\_STATUS\_THEFT\_SUSPECTED

} SystemStatus\_t;

7\. Boolean Values



Use bool for true/false states.



Examples:



bool guardModeEnabled = false;

bool findMotorcycleEnabled = false;

bool theftDetected = false;

8\. Error Handling



Sensor data should be checked before being used.



Examples:



Invalid GPS data must not be used for speed warning.

MPU6050 reading errors should not trigger theft warning immediately.

Blynk notification should not be sent continuously in every loop cycle.

9\. Event Notification Rule



Blynk events shall be limited to avoid notification spam.



Examples:



Send overspeed notification when speed first exceeds the limit.

Send repeated overspeed notification only after a defined interval.

Reset overspeed state when speed returns below the limit.

10\. Git Commit Rule



Each commit should represent one meaningful development step.



Good commit messages:



Initial project structure

Add Blynk app configuration

Add ESP32 Blynk connection test

Add GPS module

Add MPU6050 module

Add overspeed warning logic

Add theft warning logic

