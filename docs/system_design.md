\# System Design



\## 1. System Block Diagram



```text

+-------------------+

|    Blynk App      |

|  Mobile Dashboard |

+---------+---------+

&#x20;         |

&#x20;         | WiFi / Blynk Cloud

&#x20;         |

+---------v---------+

|      ESP32-S3     |

|  Main Controller  |

+----+----------+---+

&#x20;    |          |

&#x20;    | UART     | I2C

&#x20;    |          |

+----v----+  +--v------+

| NEO-8M  |  | MPU6050 |

|  GPS    |  | Sensor  |

+---------+  +---------+

&#x20;    |

&#x20;    |

+----v----------------+

| Buzzer / LED        |

| Local warning       |

+---------------------+

2\. Main Controller



ESP32-S3 is the central controller of the system.



Main tasks:



Connect to WiFi

Connect to Blynk Cloud

Read speed and location from GPS NEO-8M

Read motion or vibration data from MPU6050

Process overspeed warning

Process theft suspected warning

Update data to Blynk App

Receive control commands from Blynk App

Control buzzer or LED for local warning

3\. GPS NEO-8M Module



GPS NEO-8M communicates with ESP32-S3 using UART.



The GPS module provides:



Current speed

Latitude

Longitude



The speed value is used for overspeed detection.

The latitude and longitude values are used to create a Google Maps location link.



4\. MPU6050 Module



MPU6050 communicates with ESP32-S3 using I2C.



The MPU6050 module provides motion and vibration data.

This data is used to detect abnormal movement when guard mode is enabled.



5\. Blynk App



Blynk App is used for monitoring and control.



The app provides:



Current speed display

Speed limit setting

Guard mode switch

Find motorcycle button

GPS location display

System status display

Alert status display

Overspeed notification

Theft suspected notification

6\. System States

State	Meaning

Normal	No warning

Overspeed	Current speed exceeds speed limit

Theft suspected	Abnormal movement is detected while guard mode is enabled

7\. Firmware Modules

Module	Responsibility

config.h	Pin definitions and configuration values

blynk\_module	Blynk communication and app command handling

gps\_module	GPS reading and data processing

mpu6050\_module	MPU6050 reading and motion detection

alarm\_logic	Overspeed and theft warning logic

8\. Future Expansion



In the next phase, the system can be expanded to communicate with another embedded device through UART.



The second embedded device can be used to control buzzer, LED, or other local warning components.

