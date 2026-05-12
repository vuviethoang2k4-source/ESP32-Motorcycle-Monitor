\# Test Plan



\## 1. Blynk App Test



| Test ID | Test Item | Expected Result |

|---|---|---|

| T01 | ESP32-S3 connects to Blynk | Device becomes online |

| T02 | Send current speed to V0 | App displays current speed correctly |

| T03 | Change speed limit from app | ESP32-S3 receives new speed limit |

| T04 | Turn guard mode on/off | ESP32-S3 receives guard mode value |

| T05 | Turn find motorcycle on/off | ESP32-S3 receives find motorcycle command |

| T06 | Send latitude to V4 | App displays latitude |

| T07 | Send longitude to V5 | App displays longitude |

| T08 | Send map link to V6 | App displays Google Maps link |

| T09 | Send system status to V7 | App displays correct status |

| T10 | Send alert status to V8 | Alert widget changes state |



\## 2. Overspeed Warning Test



| Test ID | Test Condition | Expected Result |

|---|---|---|

| T11 | Current speed is lower than speed limit | System status is normal |

| T12 | Current speed is greater than speed limit | System status becomes overspeed |

| T13 | Overspeed occurs | Blynk sends overspeed notification |

| T14 | Speed returns below speed limit | System status returns to normal |



\## 3. Guard Mode Test



| Test ID | Test Condition | Expected Result |

|---|---|---|

| T15 | Guard mode is off and vibration is detected | No theft alert |

| T16 | Guard mode is on and no vibration is detected | No theft alert |

| T17 | Guard mode is on and abnormal vibration is detected | Theft alert is triggered |

| T18 | Theft alert occurs | Blynk sends theft notification |



\## 4. GPS Test



| Test ID | Test Item | Expected Result |

|---|---|---|

| T19 | GPS has valid data | Speed and location are updated |

| T20 | GPS has invalid data | Invalid data is not used for warning |



\## 5. MPU6050 Test



| Test ID | Test Item | Expected Result |

|---|---|---|

| T21 | MPU6050 is stable | No theft warning |

| T22 | MPU6050 is shaken while guard mode is on | Theft warning is triggered |



\## 6. Find Motorcycle Test



| Test ID | Test Item | Expected Result |

|---|---|---|

| T23 | Find motorcycle is turned on from app | Buzzer or LED is activated |

| T24 | Find motorcycle is turned off from app | Buzzer or LED is deactivated |

