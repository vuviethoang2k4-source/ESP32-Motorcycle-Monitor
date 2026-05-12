\# Blynk App Setup



\## 1. Template Information



Template name: ESP32 Motorcycle Monitor



Hardware: ESP32



Connection type: WiFi



\## 2. Datastreams



| Virtual Pin | Name | Type | Function |

|---|---|---|---|

| V0 | Current Speed | Integer | Display current speed |

| V1 | Speed Limit | Integer | Set maximum allowed speed |

| V2 | Guard Mode | Integer | Enable or disable guard mode |

| V3 | Find Motorcycle | Integer | Find motorcycle command |

| V4 | Latitude | Double | GPS latitude |

| V5 | Longitude | Double | GPS longitude |

| V6 | Map Link | String | Google Maps location link |

| V7 | System Status | String | Display system status |

| V8 | Alert Status | Integer | Display alert state |



\## 3. Events



| Event Code | Function |

|---|---|

| overspeed\_alert | Send notification when the motorcycle exceeds the speed limit |

| theft\_alert | Send notification when theft is suspected |



\## 4. Dashboard Widgets



| Function | Widget | Datastream |

|---|---|---|

| Current speed | Gauge | V0 |

| Speed limit | Slider or step control | V1 |

| Guard mode | Switch | V2 |

| Find motorcycle | Switch or button | V3 |

| Latitude | Label | V4 |

| Longitude | Label | V5 |

| Map link | Label | V6 |

| System status | Label | V7 |

| Alert status | LED or button | V8 |



\## 5. Status Meaning



System Status on V7:



| Text | Meaning |

|---|---|

| Binh thuong | Normal state |

| Qua toc do | Overspeed warning |

| Nghi ngo trom | Theft suspected warning |



Alert Status on V8:



| Value | Meaning |

|---|---|

| 0 | Normal |

| 1 | Alert |

