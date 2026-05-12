\# ESP32 Motorcycle Monitor



\## 1. Project Overview



This project is a motorcycle monitoring and warning system using ESP32-S3, GPS NEO-8M, MPU6050 and Blynk App.



\## 2. Main Functions



\- Display current motorcycle speed

\- Set maximum allowed speed from Blynk App

\- Enable or disable guard mode

\- Find motorcycle from app

\- Display motorcycle location

\- Show motorcycle status

\- Send notification when overspeed occurs

\- Send notification when theft is suspected



\## 3. Hardware Components



| Component | Function |

|---|---|

| ESP32-S3 | Main controller |

| GPS NEO-8M | Measure speed and location |

| MPU6050 | Detect vibration and abnormal movement |

| Buzzer or LED | Local warning and find motorcycle |

| Blynk App | Remote monitoring and control |



\## 4. Blynk Datastreams



| Virtual Pin | Name | Function |

|---|---|---|

| V0 | Current Speed | Display current speed |

| V1 | Speed Limit | Set maximum allowed speed |

| V2 | Guard Mode | Enable or disable guard mode |

| V3 | Find Motorcycle | Find motorcycle command |

| V4 | Latitude | GPS latitude |

| V5 | Longitude | GPS longitude |

| V6 | Map Link | Google Maps location link |

| V7 | System Status | Normal, overspeed, or theft suspected |

| V8 | Alert Status | Normal or alert visual status |



\## 5. Blynk Events



| Event Code | Description |

|---|---|

| overspeed\_alert | Triggered when current speed exceeds speed limit |

| theft\_alert | Triggered when motion is detected while guard mode is enabled |



\## 6. Repository Structure



ESP32-Motorcycle-Monitor/

\- README.md

\- docs/

\- firmware/

\- hardware/

\- test/



\## 7. Development Requirements



This project follows the required embedded system development workflow:



\- Use Git local repository

\- Use GitHub remote repository

\- Follow coding guidelines based on MISRA-C and Ganssle firmware development practices

\- Use modular firmware design

\- Prepare documentation and test plan

\- Optional: communicate with another embedded device through UART

