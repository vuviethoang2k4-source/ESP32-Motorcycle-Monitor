# Component List

## 1. Danh sách linh kiện chính

| STT | Linh kiện                         |    Số lượng | Chức năng                                      |
| --: | --------------------------------- | ----------: | ---------------------------------------------- |
|   1 | ESP32 DevKit V1                   |           1 | Bộ điều khiển trung tâm, kết nối WiFi và Blynk |
|   2 | GPS NEO-8M                        |           1 | Đọc tốc độ, vĩ độ, kinh độ                     |
|   3 | MPU6050                           |           1 | Phát hiện rung/lắc để chống trộm               |
|   4 | Active Buzzer                     |           1 | Cảnh báo quá tốc độ, tìm xe, chống trộm        |
|   5 | LED cảnh báo                      |           1 | Hiển thị trạng thái cảnh báo tại thiết bị      |
|   6 | Điện trở hạn dòng LED             |           1 | Hạn dòng cho LED                               |
|   7 | Pin 18650                         |    1 hoặc 2 | Nguồn cấp cho hệ thống                         |
|   8 | Đế pin 18650                      |           1 | Giữ pin và đưa dây nguồn ra ngoài              |
|   9 | Mạch sạc/tăng áp 5V cho pin 18650 |           1 | Sạc pin và tạo nguồn 5V cấp cho ESP32          |
|  10 | Công tắc gạt                      |           1 | Bật/tắt nguồn hệ thống                         |
|  11 | Dây jumper / dây điện             | Tùy nhu cầu | Kết nối các module                             |
|  12 | Breadboard hoặc PCB thử nghiệm    |           1 | Lắp mạch thử nghiệm                            |

---

## 2. ESP32 DevKit V1

ESP32 DevKit V1 là bộ điều khiển trung tâm của hệ thống.

Thông tin chính:

| Thông số                     | Giá trị                                    |
| ---------------------------- | ------------------------------------------ |
| Vi điều khiển                | ESP32                                      |
| Điện áp logic                | 3.3V                                       |
| Kết nối                      | WiFi                                       |
| Giao tiếp sử dụng            | UART, I2C, GPIO                            |
| Nguồn cấp khi thử nghiệm     | USB hoặc chân 5V/VIN                       |
| Board chọn trong Arduino IDE | ESP32 Dev Module hoặc DOIT ESP32 DEVKIT V1 |

Nhiệm vụ:

* Kết nối WiFi.
* Kết nối Blynk Cloud.
* Đọc GPS NEO-8M.
* Đọc MPU6050.
* Điều khiển buzzer và LED.
* Xử lý cảnh báo quá tốc độ.
* Xử lý chống trộm.
* Gửi dữ liệu lên Blynk App.

---

## 3. GPS NEO-8M

GPS NEO-8M được dùng để lấy dữ liệu định vị và tốc độ.

Thông tin chính:

| Thông số          | Giá trị                            |
| ----------------- | ---------------------------------- |
| Điện áp cấp       | 3.3V hoặc 5V tùy module            |
| Giao tiếp         | UART                               |
| Baudrate mặc định | 9600                               |
| Dữ liệu đầu ra    | NMEA                               |
| Dữ liệu sử dụng   | Tốc độ, vĩ độ, kinh độ, số vệ tinh |

Kết nối trong dự án:

| GPS NEO-8M | ESP32 DevKit V1 |
| ---------- | --------------- |
| VCC        | 3V3             |
| GND        | GND             |
| TX         | GPIO16 / RX2    |
| RX         | GPIO17 / TX2    |

Vai trò:

* Hiển thị tốc độ hiện tại.
* Ghi nhận tốc độ cao nhất.
* Kiểm tra quá tốc độ.
* Hiển thị vị trí xe.
* Tạo link Google Maps.
* Gửi vị trí khi có cảnh báo nghi ngờ trộm.

---

## 4. MPU6050

MPU6050 là cảm biến gia tốc và con quay hồi chuyển.

Thông tin chính:

| Thông số              | Giá trị                       |
| --------------------- | ----------------------------- |
| Điện áp cấp           | 3.3V                          |
| Giao tiếp             | I2C                           |
| Địa chỉ thường gặp    | 0x68 hoặc 0x69                |
| Dữ liệu đo            | Gia tốc, tốc độ quay          |
| Chức năng trong dự án | Phát hiện rung/lắc chống trộm |

Kết nối trong dự án:

| MPU6050 | ESP32 DevKit V1 |
| ------- | --------------- |
| VCC     | 3V3             |
| GND     | GND             |
| SDA     | GPIO21          |
| SCL     | GPIO22          |

Vai trò:

* Phát hiện xe bị rung/lắc.
* Hỗ trợ chống trộm 3 cấp.
* Kích hoạt buzzer và thông báo Blynk khi nghi ngờ trộm.

---

## 5. Active Buzzer

Buzzer được dùng để cảnh báo tại thiết bị.

Thông tin chính:

| Thông số      | Giá trị               |
| ------------- | --------------------- |
| Loại          | Active buzzer         |
| Logic đã test | HIGH = kêu, LOW = tắt |
| GPIO sử dụng  | GPIO25                |

Kết nối hiện tại nếu dùng buzzer 2 chân nhỏ:

| Buzzer | ESP32 DevKit V1 |
| ------ | --------------- |
| Chân + | GPIO25          |
| Chân - | GND             |

Kết nối nếu dùng buzzer module 3 chân:

| Buzzer module | ESP32 DevKit V1        |
| ------------- | ---------------------- |
| VCC           | 3V3 hoặc 5V tùy module |
| GND           | GND                    |
| IN / Signal   | GPIO25                 |

Ghi chú:

* Bản test có thể điều khiển trực tiếp bằng GPIO25.
* Khi thiết kế PCB, nên dùng transistor hoặc MOSFET để điều khiển buzzer an toàn hơn.

---

## 6. LED cảnh báo

LED được dùng để hiển thị trạng thái cảnh báo tại thiết bị.

| Linh kiện         | Giá trị gợi ý    |
| ----------------- | ---------------- |
| LED               | LED 3mm hoặc 5mm |
| Điện trở hạn dòng | 220Ω đến 330Ω    |
| GPIO              | GPIO26           |

Kết nối:

```text
GPIO26 → điện trở 220Ω/330Ω → LED → GND
```

---

## 7. Khối nguồn thử nghiệm

Trong giai đoạn thử nghiệm, hệ thống có thể dùng nguồn USB từ máy tính, củ sạc điện thoại hoặc nguồn pin 18650.

Phương án nguồn pin đơn giản:

```text
Pin 18650
→ mạch sạc/tăng áp 5V
→ ESP32 DevKit V1 qua chân 5V/VIN hoặc cổng USB
```

Linh kiện nguồn gợi ý:

| Linh kiện        | Thông số gợi ý                                                   |
| ---------------- | ---------------------------------------------------------------- |
| Pin 18650        | 3.7V, 2500mAh đến 3500mAh thật                                   |
| Đế pin           | Loại 1 cell nếu dùng 1 pin, hoặc 2 cell song song nếu dùng 2 pin |
| Mạch sạc/tăng áp | Hỗ trợ pin 18650 3.7V, output 5V, dòng 1A đến 2A                 |
| Công tắc         | Công tắc gạt ON/OFF, chịu dòng tối thiểu 1A                      |

Lưu ý:

* Nếu dùng 2 pin, phải dùng loại mắc song song 1S2P, không dùng loại nối tiếp 2S.
* Không cấp pin 18650 trực tiếp vào chân 3V3 của ESP32.
* Không cấp 5V vào chân 3V3 của ESP32.
* GND của nguồn, ESP32, GPS, MPU6050 và buzzer phải nối chung.

---

## 8. Khối nguồn đề xuất khi làm PCB

Khi thiết kế PCB chính thức, nên tối ưu nguồn theo hướng:

```text
Pin 18650 1S
→ mạch sạc/bảo vệ pin
→ buck-boost 3.3V
→ ESP32 + GPS + MPU6050
```

Các linh kiện nên bổ sung khi làm PCB:

| Linh kiện                           | Chức năng                      |
| ----------------------------------- | ------------------------------ |
| Buck-boost 3.3V                     | Tạo nguồn 3.3V ổn định         |
| PTC fuse                            | Bảo vệ quá dòng                |
| MOSFET chống ngược cực              | Bảo vệ khi đấu ngược pin       |
| Tụ 470uF hoặc 1000uF                | Ổn định nguồn chính            |
| Tụ 100nF                            | Lọc nguồn gần từng module      |
| Transistor/MOSFET điều khiển buzzer | Tăng dòng điều khiển còi       |
| Mạch chia áp đo pin                 | Đọc điện áp pin bằng ADC ESP32 |

---

## 9. Thư viện Arduino cần cài

| Thư viện                | Chức năng                         |
| ----------------------- | --------------------------------- |
| Blynk                   | Kết nối Blynk IoT                 |
| TinyGPSPlus             | Giải mã dữ liệu GPS NMEA          |
| Adafruit MPU6050        | Đọc cảm biến MPU6050              |
| Adafruit Unified Sensor | Thư viện phụ cho Adafruit MPU6050 |
| Adafruit BusIO          | Thư viện phụ cho Adafruit MPU6050 |

---

## 10. Ghi chú lựa chọn linh kiện

* ESP32 DevKit V1 được chọn vì dễ nạp code, dễ kết nối và phổ biến.
* GPS NEO-8M cần đặt anten hướng lên trời để bắt tín hiệu tốt.
* MPU6050 cần được cố định chắc với thân xe để phát hiện rung/lắc chính xác.
* Buzzer nên chọn loại active buzzer để điều khiển đơn giản bằng GPIO.
* Nguồn cấp cần đủ dòng, tối thiểu 5V 1A, khuyến nghị 5V 2A khi dùng ESP32 + WiFi + GPS + buzzer.
