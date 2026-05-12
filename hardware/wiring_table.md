\# Wiring Table



\## 1. ESP32-S3 and GPS NEO-8M



| GPS NEO-8M Pin | ESP32-S3 Pin | Note |

|---|---|---|

| VCC | 3.3V or 5V | Depends on GPS module board |

| GND | GND | Common ground |

| TX | GPS\_RX\_PIN | ESP32-S3 receives GPS data |

| RX | GPS\_TX\_PIN | ESP32-S3 sends data to GPS if needed |



\## 2. ESP32-S3 and MPU6050



| MPU6050 Pin | ESP32-S3 Pin | Note |

|---|---|---|

| VCC | 3.3V | Power supply |

| GND | GND | Common ground |

| SDA | I2C\_SDA\_PIN | I2C data |

| SCL | I2C\_SCL\_PIN | I2C clock |



\## 3. ESP32-S3 and Buzzer / LED



| Component | ESP32-S3 Pin | Note |

|---|---|---|

| Buzzer | BUZZER\_PIN | Local alarm and find motorcycle |

| LED | LED\_PIN | Visual warning |



\## 4. Notes



The exact GPIO pins will be updated after selecting the final ESP32-S3 development board pinout.



All modules must share the same GND.



The circuit should be tested on breadboard before PCB design.

