\# Wiring Table



\## 1. Tổng quan



File này mô tả bảng nối dây phần cứng cho dự án \*\*ESP32 Motorcycle Monitor\*\*.



Board sử dụng:



```text

ESP32 DevKit V1

```



Các module chính:



```text

GPS NEO-8M

MPU6050

Active Buzzer

LED cảnh báo

Khối nguồn pin 18650

```



\---



\## 2. GPS NEO-8M



GPS NEO-8M giao tiếp với ESP32 bằng UART2.



| GPS NEO-8M | ESP32 DevKit V1 | Ghi chú                     |

| ---------- | --------------- | --------------------------- |

| VCC        | 3V3             | Cấp nguồn cho GPS           |

| GND        | GND             | Mass chung                  |

| TX         | GPIO16 / RX2    | GPS gửi dữ liệu sang ESP32  |

| RX         | GPIO17 / TX2    | ESP32 gửi lệnh cấu hình GPS |



Quy tắc UART:



```text

GPS TX → ESP32 RX2 / GPIO16

GPS RX → ESP32 TX2 / GPIO17

```



Lưu ý:



\* Nếu chỉ đọc GPS cơ bản, có thể chỉ cần nối GPS TX vào GPIO16.

\* Trong dự án này có cấu hình GPS nhanh hơn, nên nối thêm GPS RX vào GPIO17.

\* Anten GPS nên hướng lên trời.

\* Không đặt anten GPS sát pin, dây nguồn lớn hoặc vật kim loại.



\---



\## 3. MPU6050



MPU6050 giao tiếp với ESP32 bằng I2C.



| MPU6050 | ESP32 DevKit V1 | Ghi chú        |

| ------- | --------------- | -------------- |

| VCC     | 3V3             | Cấp nguồn 3.3V |

| GND     | GND             | Mass chung     |

| SDA     | GPIO21          | Dữ liệu I2C    |

| SCL     | GPIO22          | Clock I2C      |



Lưu ý:



\* Không đấu MPU6050 vào TX/RX.

\* Nếu không nhận module, dùng I2C Scanner để kiểm tra địa chỉ `0x68` hoặc `0x69`.

\* Dây SDA/SCL không nên quá dài.

\* MPU6050 nên được cố định chắc vào thân xe để phát hiện rung/lắc chính xác.



\---



\## 4. Active Buzzer



Buzzer được dùng cho:



\* Cảnh báo quá tốc độ.

\* Tìm xe.

\* Cảnh báo chống trộm.



\### 4.1. Nếu dùng buzzer 2 chân nhỏ



| Buzzer | ESP32 DevKit V1 | Ghi chú           |

| ------ | --------------- | ----------------- |

| Chân + | GPIO25          | Điều khiển buzzer |

| Chân - | GND             | Mass chung        |



Logic đã test:



```text

GPIO25 = HIGH → buzzer kêu

GPIO25 = LOW  → buzzer tắt

```



\### 4.2. Nếu dùng buzzer module 3 chân



| Buzzer module | ESP32 DevKit V1 | Ghi chú         |

| ------------- | --------------- | --------------- |

| VCC           | 3V3 hoặc 5V     | Tùy loại module |

| GND           | GND             | Mass chung      |

| IN / Signal   | GPIO25          | Chân điều khiển |



Ghi chú:



\* Khi test nhanh, có thể điều khiển buzzer trực tiếp bằng GPIO25.

\* Khi làm PCB, nên dùng transistor hoặc MOSFET để điều khiển buzzer.



\---



\## 5. LED cảnh báo



LED được dùng để hiển thị trạng thái cảnh báo tại thiết bị.



| LED         | ESP32 DevKit V1               | Ghi chú        |

| ----------- | ----------------------------- | -------------- |

| Anode LED   | GPIO26 qua điện trở 220Ω/330Ω | Chân dương LED |

| Cathode LED | GND                           | Chân âm LED    |



Sơ đồ:



```text

GPIO26 → điện trở 220Ω/330Ω → LED → GND

```



\---



\## 6. Khối nguồn khi test



Có thể cấp nguồn cho ESP32 bằng một trong các cách sau:



\### 6.1. Cấp qua USB



```text

Củ sạc điện thoại / sạc dự phòng / máy tính

→ cổng USB ESP32 DevKit V1

```



Cách này dễ và an toàn khi test.



\---



\### 6.2. Cấp bằng pin 18650 và mạch sạc/tăng áp 5V



Sơ đồ nguồn:



```text

Pin 18650

→ mạch sạc/tăng áp 5V

→ ESP32 DevKit V1

```



Đấu dây:



| Mạch nguồn 5V | ESP32 DevKit V1 |

| ------------- | --------------- |

| 5V / OUT+     | 5V hoặc VIN     |

| GND / OUT-    | GND             |



Lưu ý:



\* Không cấp 5V vào chân 3V3.

\* Không cấp pin 18650 trực tiếp vào chân 3V3.

\* Nếu dùng 2 viên 18650, phải dùng cấu hình song song 1S2P.

\* Không dùng đế pin nối tiếp 2S với mạch sạc 1S.



\---



\## 7. Công tắc nguồn 3 chân



Công tắc gạt 3 chân thường là loại SPDT.



Nếu chỉ dùng để bật/tắt nguồn, dùng 2 chân:



```text

Chân giữa = COM

Một chân bên = OUT

Chân bên còn lại bỏ trống

```



Đấu ở dây dương nguồn:



| Nguồn / công tắc      | Nối tới            |

| --------------------- | ------------------ |

| 5V OUT từ mạch nguồn  | Chân giữa công tắc |

| Một chân bên công tắc | ESP32 5V/VIN       |

| GND nguồn             | ESP32 GND          |



Sơ đồ:



```text

5V OUT nguồn → chân giữa công tắc

chân bên công tắc → ESP32 5V/VIN



GND nguồn → ESP32 GND

```



Nếu bật/tắt bị ngược hướng mong muốn, đổi dây sang chân bên còn lại của công tắc.



\---



\## 8. Bảng chân tổng hợp



| Chức năng         | Module        | Chân module        | ESP32 DevKit V1 |

| ----------------- | ------------- | ------------------ | --------------- |

| GPS nguồn         | GPS NEO-8M    | VCC                | 3V3             |

| GPS GND           | GPS NEO-8M    | GND                | GND             |

| GPS UART RX ESP32 | GPS NEO-8M    | TX                 | GPIO16 / RX2    |

| GPS UART TX ESP32 | GPS NEO-8M    | RX                 | GPIO17 / TX2    |

| MPU nguồn         | MPU6050       | VCC                | 3V3             |

| MPU GND           | MPU6050       | GND                | GND             |

| MPU I2C data      | MPU6050       | SDA                | GPIO21          |

| MPU I2C clock     | MPU6050       | SCL                | GPIO22          |

| Buzzer            | Active buzzer | + hoặc IN          | GPIO25          |

| Buzzer GND        | Active buzzer | - hoặc GND         | GND             |

| LED cảnh báo      | LED           | Anode qua điện trở | GPIO26          |

| LED GND           | LED           | Cathode            | GND             |

| Nguồn chính       | Mạch nguồn 5V | 5V / OUT+          | 5V / VIN        |

| Nguồn GND         | Mạch nguồn    | GND / OUT-         | GND             |



\---



\## 9. Lưu ý nối GND



Tất cả các khối phải chung GND:



```text

GND ESP32

GND GPS

GND MPU6050

GND buzzer

GND LED

GND nguồn

```



Nếu không chung GND, UART GPS và I2C MPU6050 có thể hoạt động sai hoặc không hoạt động.



\---



\## 10. Lưu ý khi nạp code



Khi nạp code bằng USB máy tính:



\* Có thể rút nguồn pin ra để tránh xung nguồn.

\* Chọn đúng board trong Arduino IDE:



&#x20; \* `ESP32 Dev Module`

&#x20; \* hoặc `DOIT ESP32 DEVKIT V1`

\* Chọn đúng cổng COM.

\* Nếu không upload được, giữ nút `BOOT` khi bắt đầu upload.



\---



\## 11. Lưu ý khi làm PCB



Khi chuyển sang PCB, nên cải tiến phần cứng như sau:



\* Dùng transistor hoặc MOSFET điều khiển buzzer.

\* Thêm tụ 470uF hoặc 1000uF ở đường nguồn chính.

\* Thêm tụ 100nF gần ESP32, GPS và MPU6050.

\* Thêm mạch bảo vệ ngược cực nguồn.

\* Thêm cầu chì tự phục hồi PTC.

\* Thiết kế vị trí anten GPS thoáng, không bị che bởi kim loại.

\* Cố định MPU6050 chắc chắn với thân xe hoặc hộp thiết bị.



