\# Project Description



\## 1. Project Name



ESP32 Motorcycle Monitor



\## 2. Objective



The objective of this project is to design a motorcycle monitoring and warning system using ESP32-S3, GPS NEO-8M, MPU6050, and Blynk App.



The system monitors motorcycle speed and location, allows the user to set a speed limit, enables guard mode, and sends warning notifications to the user.



\## 3. Main Functions



\- Display current speed

\- Set maximum allowed speed from Blynk App

\- Enable or disable guard mode

\- Find motorcycle from app

\- Display motorcycle location

\- Display system status

\- Send notification when overspeed occurs

\- Send notification when theft is suspected



\## 4. Current Development Scope



The current development phase focuses on the central device and Blynk App.



Current scope:



```text

ESP32-S3 + GPS NEO-8M + MPU6050 + Blynk App



Communication with another embedded device will be added in a later phase.



5\. System Inputs

Input	Source

Current speed	GPS NEO-8M

Latitude and longitude	GPS NEO-8M

Motion or vibration	MPU6050

Speed limit	Blynk App

Guard mode command	Blynk App

Find motorcycle command	Blynk App

6\. System Outputs

Output	Destination

Current speed	Blynk App

Location information	Blynk App

System status	Blynk App

Alert status	Blynk App

Overspeed notification	Blynk App

Theft notification	Blynk App

Buzzer or LED signal	Local warning circuit

7\. Warning Logic

Overspeed Warning



If the current speed is greater than the speed limit, the system changes its status to overspeed and sends an overspeed notification.



Theft Warning



If guard mode is enabled and abnormal motion or vibration is detected by MPU6050, the system changes its status to theft suspected and sends a theft warning notification.

