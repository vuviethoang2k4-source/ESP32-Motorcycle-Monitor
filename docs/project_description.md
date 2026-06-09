# Project Description

## 1. Tên dự án

**ESP32 Motorcycle Monitor**

Hệ thống giám sát xe máy sử dụng **ESP32 DevKit V1**, **GPS NEO-8M**, **MPU6050**, **buzzer** và **Blynk IoT App**.

---

## 2. Mục tiêu dự án

Dự án được xây dựng nhằm tạo ra một hệ thống giám sát xe máy có khả năng:

* Theo dõi tốc độ hiện tại của xe.
* Cảnh báo khi xe vượt quá tốc độ giới hạn.
* Hiển thị vị trí xe thông qua GPS.
* Cung cấp link Google Maps để xem vị trí xe.
* Ghi nhận tốc độ cao nhất đã đạt được.
* Cho phép người dùng reset tốc độ cao nhất.
* Phát hiện rung/lắc bất thường để hỗ trợ chống trộm.
* Gửi cảnh báo về điện thoại thông qua Blynk App.
* Kích hoạt còi cảnh báo tại xe khi có sự kiện bất thường.

---

## 3. Phạm vi hiện tại

Phiên bản hiện tại tập trung vào **khối trung tâm và ứng dụng Blynk**.

Các chức năng đã triển khai gồm:

| Chức năng                             | Trạng thái             |
| ------------------------------------- | ---------------------- |
| ESP32 DevKit V1 kết nối WiFi          | Đã hoàn thành          |
| ESP32 kết nối Blynk App               | Đã hoàn thành          |
| Đọc tốc độ từ GPS NEO-8M              | Đã hoàn thành          |
| Đọc vĩ độ và kinh độ từ GPS           | Đã hoàn thành          |
| Hiển thị link Google Maps             | Đã hoàn thành          |
| Cài đặt tốc độ giới hạn từ app        | Đã hoàn thành          |
| Cảnh báo quá tốc độ                   | Đã hoàn thành          |
| Buzzer cảnh báo quá tốc độ            | Đã hoàn thành          |
| Hiển thị tốc độ cao nhất              | Đã hoàn thành          |
| Reset tốc độ cao nhất từ app          | Đã hoàn thành          |
| MPU6050 phát hiện rung/lắc            | Đã hoàn thành          |
| Chống trộm 3 cấp                      | Đã hoàn thành          |
| Gửi cảnh báo nghi ngờ trộm kèm vị trí | Đã hoàn thành          |
| Khối nguồn pin 18650                  | Đang hoàn thiện        |
| PCB                                   | Dự kiến phát triển sau |

---

## 4. Phần cứng sử dụng

| Linh kiện           | Vai trò                                                 |
| ------------------- | ------------------------------------------------------- |
| ESP32 DevKit V1     | Bộ điều khiển trung tâm, xử lý dữ liệu và kết nối Blynk |
| GPS NEO-8M          | Cung cấp tốc độ, vĩ độ và kinh độ                       |
| MPU6050             | Phát hiện rung/lắc, hỗ trợ chống trộm                   |
| Active Buzzer       | Cảnh báo tại xe                                         |
| LED                 | Hiển thị trạng thái cảnh báo                            |
| Pin 18650           | Nguồn cấp cho hệ thống                                  |
| Mạch sạc/tăng áp 5V | Sạc pin và cấp nguồn cho ESP32                          |

---

## 5. Chức năng theo từng khối

### 5.1. Khối ESP32 DevKit V1

ESP32 DevKit V1 là khối trung tâm của hệ thống.

Nhiệm vụ chính:

* Kết nối WiFi.
* Kết nối Blynk Cloud.
* Đọc dữ liệu GPS.
* Đọc dữ liệu MPU6050.
* Xử lý cảnh báo quá tốc độ.
* Xử lý cảnh báo chống trộm.
* Điều khiển buzzer và LED.
* Gửi dữ liệu lên Blynk App.

---

### 5.2. Khối GPS NEO-8M

GPS NEO-8M được dùng để lấy:

* Tốc độ hiện tại của xe.
* Vĩ độ.
* Kinh độ.
* Số vệ tinh bắt được.

Dữ liệu GPS được ESP32 xử lý để:

* Hiển thị tốc độ hiện tại.
* Kiểm tra quá tốc độ.
* Tạo link Google Maps.
* Gửi vị trí khi có cảnh báo nghi ngờ trộm.

---

### 5.3. Khối MPU6050

MPU6050 được dùng để phát hiện rung/lắc bất thường của xe.

Khi người dùng bật **Guard Mode** trên Blynk App, ESP32 sẽ bắt đầu giám sát dữ liệu từ MPU6050.

Logic chống trộm hiện tại:

| Số lần tác động | Phản ứng                              |
| --------------- | ------------------------------------- |
| Lần 1           | Buzzer bíp 1 lần                      |
| Lần 2           | Buzzer bíp 2 lần                      |
| Lần 3           | Buzzer kêu mạnh và gửi cảnh báo Blynk |

Sau một khoảng thời gian không có tác động mới, hệ thống sẽ reset số lần cảnh báo về 0.

---

### 5.4. Khối Blynk App

Blynk App được dùng làm giao diện điều khiển và giám sát từ xa.

Người dùng có thể:

* Xem tốc độ hiện tại.
* Đặt tốc độ giới hạn.
* Bật/tắt chế độ chống trộm.
* Bật/tắt chức năng tìm xe.
* Xem vĩ độ, kinh độ và link Google Maps.
* Xem trạng thái hệ thống.
* Xem tốc độ cao nhất.
* Reset tốc độ cao nhất.

---

### 5.5. Khối cảnh báo

Hệ thống có hai loại cảnh báo chính:

#### Cảnh báo quá tốc độ

Điều kiện:

```text
currentSpeedKmh > speedLimitKmh
```

Phản ứng:

* Cập nhật trạng thái trên Blynk.
* Bật buzzer.
* Gửi event `overspeed_alert`.

Thông báo quá tốc độ không gửi kèm link Google Maps.

#### Cảnh báo nghi ngờ trộm

Điều kiện:

```text
Guard Mode = ON
MPU6050 phát hiện rung/lắc bất thường 3 lần
```

Phản ứng:

* Buzzer kêu mạnh.
* Cập nhật trạng thái `Nghi ngo trom`.
* Gửi event `theft_alert`.
* Gửi kèm link Google Maps.

---

## 6. Blynk Datastream

| Virtual Pin | Tên             | Chức năng             |
| ----------- | --------------- | --------------------- |
| V0          | Current Speed   | Tốc độ hiện tại       |
| V1          | Speed Limit     | Tốc độ giới hạn       |
| V2          | Guard Mode      | Bật/tắt chống trộm    |
| V3          | Find Motorcycle | Bật/tắt tìm xe        |
| V4          | Latitude        | Vĩ độ                 |
| V5          | Longitude       | Kinh độ               |
| V6          | Map Link        | Link Google Maps      |
| V7          | System Status   | Trạng thái hệ thống   |
| V8          | Alert Status    | Trạng thái cảnh báo   |
| V9          | Max Speed       | Tốc độ cao nhất       |
| V10         | Reset Max Speed | Reset tốc độ cao nhất |

---

## 7. Blynk Events

| Event code      | Ý nghĩa                              |
| --------------- | ------------------------------------ |
| overspeed_alert | Cảnh báo xe vượt quá tốc độ giới hạn |
| theft_alert     | Cảnh báo nghi ngờ trộm               |

---

## 8. Trạng thái hệ thống

Các trạng thái có thể hiển thị trên Blynk App:

| Trạng thái     | Ý nghĩa                        |
| -------------- | ------------------------------ |
| Dang doi GPS   | GPS chưa có dữ liệu hợp lệ     |
| Binh thuong    | Hệ thống hoạt động bình thường |
| Qua toc do     | Xe vượt quá tốc độ giới hạn    |
| Dang bao ve    | Chế độ chống trộm đang bật     |
| Canh bao muc 1 | Phát hiện tác động lần 1       |
| Canh bao muc 2 | Phát hiện tác động lần 2       |
| Nghi ngo trom  | Phát hiện tác động mức 3       |

---

## 9. Nguồn cấp

Trong giai đoạn thử nghiệm, hệ thống có thể được cấp nguồn qua:

* Cổng USB của ESP32.
* Mạch sạc/tăng áp 5V dùng pin 18650.

Phương án nguồn đề xuất cho bản demo:

```text
Pin 18650
→ mạch sạc/tăng áp 5V
→ ESP32 DevKit V1
```

Phương án nguồn tối ưu khi thiết kế PCB:

```text
Pin 18650 1S
→ mạch sạc/bảo vệ pin
→ buck-boost 3.3V
→ ESP32 + GPS + MPU6050
```

---

## 10. Hướng phát triển tiếp theo

Các phần có thể cải tiến thêm:

* Thêm hiển thị phần trăm pin.
* Thêm nút tắt cảnh báo thủ công.
* Tối ưu mạch điều khiển buzzer bằng transistor.
* Thiết kế PCB.
* Tối ưu vị trí đặt anten GPS.
* Hoàn thiện vỏ hộp cho thiết bị.
