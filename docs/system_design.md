# System Design

## 1. Tổng quan hệ thống

Hệ thống **ESP32 Motorcycle Monitor** được thiết kế để giám sát xe máy thông qua ESP32 DevKit V1, GPS NEO-8M, MPU6050, buzzer và Blynk IoT App.

Hệ thống gồm các khối chính:

```text
GPS NEO-8M
    ↓
ESP32 DevKit V1
    ↓
Blynk Cloud
    ↓
Blynk App

MPU6050
    ↓
ESP32 DevKit V1
    ↓
Buzzer / Blynk Alert
```

ESP32 đóng vai trò là bộ xử lý trung tâm. Thiết bị đọc dữ liệu từ GPS và MPU6050, xử lý các điều kiện cảnh báo, điều khiển buzzer và gửi dữ liệu lên Blynk App.

---

## 2. Sơ đồ khối hệ thống

```text
+-------------------+        UART        +----------------------+
|   GPS NEO-8M      | -----------------> |                      |
| Speed / Location  |                    |                      |
+-------------------+                    |                      |
                                         |                      |
+-------------------+        I2C         |   ESP32 DevKit V1    |       WiFi
|     MPU6050       | -----------------> |                      | ----------------+
| Accel / Gyro      |                    |                      |                 |
+-------------------+                    |                      |                 |
                                         |                      |                 |
+-------------------+                    |                      |                 |
| Buzzer + LED      | <----------------- |                      |                 |
| Local Alarm       |       GPIO         +----------------------+                 |
+-------------------+                                                             |
                                                                                  |
                                                                                  v
                                                                       +------------------+
                                                                       |   Blynk Cloud    |
                                                                       +------------------+
                                                                                  |
                                                                                  v
                                                                       +------------------+
                                                                       |   Blynk App      |
                                                                       +------------------+
```

---

## 3. Vai trò từng khối

### 3.1. ESP32 DevKit V1

ESP32 DevKit V1 là khối trung tâm của hệ thống.

Nhiệm vụ:

* Kết nối WiFi.
* Kết nối Blynk Cloud.
* Đọc dữ liệu GPS qua UART.
* Đọc dữ liệu MPU6050 qua I2C.
* Tính toán tốc độ hiện tại.
* Ghi nhận tốc độ cao nhất.
* Kiểm tra quá tốc độ.
* Xử lý chống trộm 3 cấp.
* Điều khiển buzzer và LED.
* Gửi dữ liệu lên Blynk App.

---

### 3.2. GPS NEO-8M

GPS NEO-8M cung cấp dữ liệu:

* Tốc độ hiện tại.
* Vĩ độ.
* Kinh độ.
* Số vệ tinh.

Giao tiếp với ESP32 bằng UART2:

| GPS NEO-8M | ESP32 DevKit V1 |
| ---------- | --------------- |
| TX         | GPIO16 / RX2    |
| RX         | GPIO17 / TX2    |

Dữ liệu GPS được dùng cho:

* Hiển thị tốc độ hiện tại.
* Kiểm tra quá tốc độ.
* Tạo link Google Maps.
* Gửi vị trí khi có cảnh báo nghi ngờ trộm.

---

### 3.3. MPU6050

MPU6050 cung cấp dữ liệu:

* Gia tốc theo các trục X, Y, Z.
* Tốc độ quay theo các trục X, Y, Z.

Giao tiếp với ESP32 bằng I2C:

| MPU6050 | ESP32 DevKit V1 |
| ------- | --------------- |
| SDA     | GPIO21          |
| SCL     | GPIO22          |

Dữ liệu MPU6050 được dùng cho chức năng chống trộm.

Khi **Guard Mode** bật, ESP32 đọc dữ liệu MPU6050 và phát hiện rung/lắc bất thường. Hệ thống sử dụng logic 3 cấp:

| Số lần tác động | Phản ứng                                     |
| --------------- | -------------------------------------------- |
| Lần 1           | Buzzer bíp 1 lần                             |
| Lần 2           | Buzzer bíp 2 lần                             |
| Lần 3           | Buzzer kêu mạnh 5 giây và gửi cảnh báo Blynk |

---

### 3.4. Buzzer và LED

Buzzer và LED là khối cảnh báo tại thiết bị.

Buzzer được dùng cho:

* Cảnh báo quá tốc độ.
* Tìm xe.
* Cảnh báo chống trộm.

LED được dùng để hiển thị trạng thái cảnh báo tại thiết bị.

| Thiết bị | GPIO   |
| -------- | ------ |
| Buzzer   | GPIO25 |
| LED      | GPIO26 |

---

### 3.5. Blynk App

Blynk App là giao diện điều khiển và giám sát từ xa.

Người dùng có thể:

* Xem tốc độ hiện tại.
* Đặt tốc độ giới hạn.
* Bật/tắt chống trộm.
* Bật/tắt tìm xe.
* Xem vị trí xe.
* Xem trạng thái hệ thống.
* Xem tốc độ cao nhất.
* Reset tốc độ cao nhất.

---

## 4. Luồng dữ liệu GPS

```text
GPS NEO-8M
    ↓ UART
ESP32 đọc chuỗi NMEA
    ↓
TinyGPSPlus giải mã dữ liệu
    ↓
Lấy speed, latitude, longitude, satellites
    ↓
ESP32 xử lý
    ↓
Gửi dữ liệu lên Blynk
```

Dữ liệu GPS được cập nhật vào biến trạng thái:

```text
currentSpeedKmh
latestGpsData.latitude
latestGpsData.longitude
latestGpsData.satellites
```

---

## 5. Luồng xử lý quá tốc độ

Điều kiện:

```text
currentSpeedKmh > speedLimitKmh
```

Luồng xử lý:

```text
Đọc tốc độ GPS
    ↓
So sánh với tốc độ giới hạn
    ↓
Nếu vượt quá giới hạn
    ↓
Bật buzzer
Cập nhật trạng thái "Qua toc do"
Gửi event overspeed_alert
```

Quy ước:

* Cảnh báo quá tốc độ không gửi kèm link Google Maps.
* Còi được xử lý trực tiếp trên ESP32 nên phản ứng nhanh hơn app.
* Blynk App có thể có độ trễ do phụ thuộc WiFi và Blynk Cloud.

---

## 6. Luồng xử lý Max Speed

Hệ thống ghi nhận tốc độ cao nhất từ khi ESP32 khởi động.

Luồng xử lý:

```text
Đọc currentSpeedKmh
    ↓
Nếu currentSpeedKmh > maxSpeedKmh
    ↓
Cập nhật maxSpeedKmh
    ↓
Gửi maxSpeedKmh lên Blynk V9
```

Điều kiện lọc:

```text
currentSpeedKmh <= 150 km/h
```

Điều kiện này giúp tránh trường hợp GPS lỗi làm tốc độ nhảy ảo quá cao.

Reset Max Speed:

```text
Người dùng bấm V10
    ↓
ESP32 đặt maxSpeedKmh = 0
    ↓
Gửi V9 = 0 lên Blynk
```

---

## 7. Luồng xử lý chống trộm

Chống trộm chỉ hoạt động khi:

```text
Guard Mode = ON
```

Khi Guard Mode bật:

```text
ESP32 reset trạng thái chống trộm
    ↓
MPU6050 lấy mốc trạng thái đứng yên
    ↓
ESP32 bắt đầu giám sát rung/lắc
```

Logic phát hiện tác động:

```text
Đọc gia tốc và gyro
    ↓
Tính accelDeltaG và gyroDps
    ↓
Nếu vượt ngưỡng
    ↓
Tính là một lần tác động
```

Logic cảnh báo:

```text
Lần 1:
    Buzzer bíp 1 lần
    Status = Canh bao muc 1

Lần 2:
    Buzzer bíp 2 lần
    Status = Canh bao muc 2

Lần 3:
    Buzzer kêu mạnh 5 giây
    Status = Nghi ngo trom
    Gửi theft_alert kèm link Google Maps
```

Sau một khoảng thời gian không có tác động mới, hệ thống reset số lần tác động về 0.

---

## 8. Ưu tiên trạng thái hệ thống

Khi nhiều sự kiện xảy ra cùng lúc, trạng thái hiển thị trên Blynk được ưu tiên theo thứ tự:

| Ưu tiên | Trạng thái     |
| ------- | -------------- |
| 1       | Nghi ngo trom  |
| 2       | Canh bao muc 2 |
| 3       | Canh bao muc 1 |
| 4       | Qua toc do     |
| 5       | Dang bao ve    |
| 6       | Dang doi GPS   |
| 7       | Binh thuong    |

Lý do: cảnh báo trộm quan trọng hơn cảnh báo quá tốc độ, vì nó liên quan đến an toàn tài sản.

---

## 9. Blynk Datastream

| Virtual Pin | Tên             | Kiểu dữ liệu | Chức năng             |
| ----------- | --------------- | ------------ | --------------------- |
| V0          | Current Speed   | Integer      | Tốc độ hiện tại       |
| V1          | Speed Limit     | Integer      | Tốc độ giới hạn       |
| V2          | Guard Mode      | Integer      | Bật/tắt chống trộm    |
| V3          | Find Motorcycle | Integer      | Bật/tắt tìm xe        |
| V4          | Latitude        | Double       | Vĩ độ                 |
| V5          | Longitude       | Double       | Kinh độ               |
| V6          | Map Link        | String       | Link Google Maps      |
| V7          | System Status   | String       | Trạng thái hệ thống   |
| V8          | Alert Status    | Integer      | Trạng thái cảnh báo   |
| V9          | Max Speed       | Integer      | Tốc độ cao nhất       |
| V10         | Reset Max Speed | Integer      | Reset tốc độ cao nhất |

---

## 10. Blynk Events

| Event code      | Điều kiện kích hoạt                     | Nội dung                                   |
| --------------- | --------------------------------------- | ------------------------------------------ |
| overspeed_alert | Tốc độ hiện tại lớn hơn tốc độ giới hạn | Tốc độ hiện tại và tốc độ giới hạn         |
| theft_alert     | MPU6050 phát hiện tác động mức 3        | Cảnh báo nghi ngờ trộm và link Google Maps |

---

## 11. Thiết kế nguồn

### Bản thử nghiệm

Trong bản thử nghiệm, ESP32 DevKit V1 có thể được cấp nguồn qua:

```text
Pin 18650
→ mạch sạc/tăng áp 5V
→ ESP32 chân 5V/VIN hoặc cổng USB
```

### Bản PCB đề xuất

Khi thiết kế PCB, nên tối ưu nguồn theo hướng:

```text
Pin 18650 1S
→ mạch sạc/bảo vệ pin
→ buck-boost 3.3V
→ ESP32 + GPS + MPU6050
```

Cách này tiết kiệm năng lượng hơn so với việc tăng lên 5V rồi hạ xuống 3.3V.

---

## 12. Chu kỳ xử lý trong firmware

Các tác vụ chính được chạy bằng `BlynkTimer`.

| Tác vụ                  |   Chu kỳ | Chức năng                            |
| ----------------------- | -------: | ------------------------------------ |
| taskReadGps             |    20 ms | Đọc UART GPS                         |
| taskReadMpu6050         |    50 ms | Đọc MPU6050                          |
| taskUpdateBlynkSpeed    |   500 ms | Gửi tốc độ và max speed khi thay đổi |
| taskCheckOverspeed      |   200 ms | Kiểm tra quá tốc độ                  |
| taskHandleTheftWarning  |   100 ms | Xử lý chống trộm                     |
| taskUpdateSystemStatus  |   500 ms | Cập nhật trạng thái                  |
| taskUpdateBlynkLocation | 30000 ms | Gửi vị trí và link map               |
| taskPrintDebug          |  3000 ms | In debug ra Serial Monitor           |

---

## 13. Ghi chú thiết kế

* Không gửi dữ liệu Blynk quá nhanh để tránh thiết bị bị offline.
* Tốc độ và max speed chỉ gửi khi giá trị thay đổi.
* Vị trí và link Google Maps gửi chậm hơn vì không cần realtime như tốc độ.
* Còi được xử lý trực tiếp trên ESP32 nên không phụ thuộc độ trễ của Blynk.
* GPS cần đặt ở vị trí thoáng, anten hướng lên trời.
* MPU6050 cần được cố định chắc vào thân xe để phát hiện rung/lắc chính xác.
