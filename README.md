\# ESP32 Motorcycle Monitor



Dự án giám sát xe máy sử dụng \*\*ESP32 DevKit V1\*\*, \*\*GPS NEO-8M\*\*, \*\*MPU6050\*\*, \*\*buzzer\*\* và \*\*Blynk IoT App\*\*.



Hệ thống có các chức năng chính:



\* Hiển thị tốc độ hiện tại của xe.

\* Cài đặt tốc độ giới hạn từ Blynk App.

\* Cảnh báo quá tốc độ bằng thông báo Blynk và còi buzzer.

\* Hiển thị vị trí xe bằng vĩ độ, kinh độ và link Google Maps.

\* Ghi nhận tốc độ cao nhất đã đạt được.

\* Reset tốc độ cao nhất từ app.

\* Bật/tắt chế độ chống trộm từ app.

\* Phát hiện rung/lắc bằng MPU6050.

\* Chống trộm 3 cấp:



&#x20; \* Lần 1: còi bíp nhẹ 1 lần.

&#x20; \* Lần 2: còi bíp nhẹ 2 lần.

&#x20; \* Lần 3: còi kêu mạnh và gửi thông báo kèm vị trí xe.

\* Tìm xe bằng cách bật còi từ Blynk App.



\---



\## 1. Phần cứng sử dụng



| Linh kiện           | Chức năng                                      |

| ------------------- | ---------------------------------------------- |

| ESP32 DevKit V1     | Vi điều khiển trung tâm, kết nối WiFi và Blynk |

| GPS NEO-8M          | Đọc tốc độ, vĩ độ, kinh độ                     |

| MPU6050             | Phát hiện rung/lắc, hỗ trợ chống trộm          |

| Active Buzzer       | Cảnh báo quá tốc độ, tìm xe, chống trộm        |

| LED cảnh báo        | Hiển thị trạng thái cảnh báo tại thiết bị      |

| Pin 18650           | Nguồn cấp cho hệ thống                         |

| Mạch sạc/tăng áp 5V | Sạc pin và cấp nguồn 5V cho ESP32              |



\---



\## 2. Bảng chân kết nối



\### GPS NEO-8M



| GPS NEO-8M | ESP32 DevKit V1 |

| ---------- | --------------- |

| VCC        | 3V3             |

| GND        | GND             |

| TX         | GPIO16 / RX2    |

| RX         | GPIO17 / TX2    |



\### MPU6050



| MPU6050 | ESP32 DevKit V1 |

| ------- | --------------- |

| VCC     | 3V3             |

| GND     | GND             |

| SDA     | GPIO21          |

| SCL     | GPIO22          |



\### Buzzer và LED



| Thiết bị | ESP32 DevKit V1 |

| -------- | --------------- |

| Buzzer   | GPIO25          |

| LED      | GPIO26          |



\---



\## 3. Blynk Datastream



| Virtual Pin | Tên             | Chức năng                    |

| ----------- | --------------- | ---------------------------- |

| V0          | Current Speed   | Hiển thị tốc độ hiện tại     |

| V1          | Speed Limit     | Cài đặt tốc độ giới hạn      |

| V2          | Guard Mode      | Bật/tắt chế độ chống trộm    |

| V3          | Find Motorcycle | Bật/tắt còi tìm xe           |

| V4          | Latitude        | Hiển thị vĩ độ               |

| V5          | Longitude       | Hiển thị kinh độ             |

| V6          | Map Link        | Hiển thị link Google Maps    |

| V7          | System Status   | Hiển thị trạng thái hệ thống |

| V8          | Alert Status    | Hiển thị trạng thái cảnh báo |

| V9          | Max Speed       | Hiển thị tốc độ cao nhất     |

| V10         | Reset Max Speed | Reset tốc độ cao nhất        |



\---



\## 4. Blynk Events



| Event code      | Chức năng                                |

| --------------- | ---------------------------------------- |

| overspeed\_alert | Gửi cảnh báo khi xe vượt tốc độ giới hạn |

| theft\_alert     | Gửi cảnh báo khi phát hiện nghi ngờ trộm |



Quy ước thông báo:



\* `overspeed\_alert`: chỉ gửi tốc độ hiện tại và tốc độ giới hạn.

\* `theft\_alert`: gửi cảnh báo nghi ngờ trộm và kèm link Google Maps.



\---



\## 5. Cấu trúc thư mục



```text

ESP32-Motorcycle-Monitor/

├── docs/

│   ├── blynk\_setup.md

│   ├── project\_description.md

│   └── system\_design.md

├── firmware/

│   ├── esp32\_main/

│   │   ├── esp32\_main.ino

│   │   ├── config.h

│   │   ├── secrets\_example.h

│   │   ├── gps\_module.h

│   │   ├── gps\_module.cpp

│   │   ├── mpu6050\_module.h

│   │   └── mpu6050\_module.cpp

│   └── mpu6050\_test/

│       └── mpu6050\_test.ino

├── hardware/

│   ├── component\_list.md

│   └── wiring\_table.md

├── test/

│   ├── test\_plan.md

│   └── test\_results.md

├── .gitignore

└── README.md

```



\---



\## 6. Firmware chính



Firmware chính nằm tại:



```text

firmware/esp32\_main/esp32\_main.ino

```



Các file quan trọng:



| File                                  | Chức năng                                      |

| ------------------------------------- | ---------------------------------------------- |

| esp32\_main.ino                        | Chương trình chính                             |

| config.h                              | Cấu hình Virtual Pin, GPIO, hằng số            |

| secrets.h                             | WiFi và Blynk token thật, không đẩy lên GitHub |

| secrets\_example.h                     | File mẫu cho cấu hình WiFi/Blynk               |

| gps\_module.h / gps\_module.cpp         | Module đọc GPS NEO-8M                          |

| mpu6050\_module.h / mpu6050\_module.cpp | Module đọc MPU6050 và xử lý chống trộm         |



\---



\## 7. Cách cấu hình secrets.h



Tạo file:



```text

firmware/esp32\_main/secrets.h

```



Dựa theo mẫu:



```text

firmware/esp32\_main/secrets\_example.h

```



Nội dung cần có:



```cpp

\#define BLYNK\_TEMPLATE\_ID "YourTemplateID"

\#define BLYNK\_TEMPLATE\_NAME "YourTemplateName"

\#define BLYNK\_AUTH\_TOKEN "YourAuthToken"



\#define WIFI\_SSID "YourWiFiName"

\#define WIFI\_PASSWORD "YourWiFiPassword"

```



Lưu ý: `secrets.h` chứa thông tin thật nên không được commit lên GitHub.



\---



\## 8. Cách chạy



1\. Mở Arduino IDE.

2\. Mở file:



```text

firmware/esp32\_main/esp32\_main.ino

```



3\. Chọn board:



```text

ESP32 Dev Module

```



hoặc:



```text

DOIT ESP32 DEVKIT V1

```



4\. Chọn đúng cổng COM.

5\. Cài các thư viện cần thiết:



&#x20;  \* Blynk

&#x20;  \* TinyGPSPlus

&#x20;  \* Adafruit MPU6050

&#x20;  \* Adafruit Unified Sensor

&#x20;  \* Adafruit BusIO

6\. Verify và Upload.

7\. Mở Serial Monitor ở baudrate `115200`.



\---



\## 9. Trạng thái hiện tại



Các chức năng đã triển khai:



\* ESP32 DevKit V1 kết nối Blynk.

\* GPS NEO-8M đọc được tốc độ và vị trí thật.

\* Hiển thị tốc độ, tọa độ và link Google Maps trên app.

\* Cảnh báo quá tốc độ bằng Blynk notification và buzzer.

\* Ghi nhận tốc độ cao nhất.

\* Reset tốc độ cao nhất bằng Blynk App.

\* MPU6050 phát hiện rung/lắc.

\* Chống trộm 3 cấp.

\* Cảnh báo nghi ngờ trộm có gửi kèm link vị trí.



\---



\## 10. Hướng phát triển tiếp theo



\* Hoàn thiện khối nguồn dùng pin 18650.

\* Thêm hiển thị phần trăm pin.

\* Tối ưu mạch buzzer bằng transistor.

\* Thiết kế PCB cho hệ thống.

\* Tối ưu vị trí đặt anten GPS để bắt tín hiệu nhanh hơn.



