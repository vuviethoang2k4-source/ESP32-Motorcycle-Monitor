\# Blynk Setup



\## 1. Mục đích



File này mô tả cách cấu hình \*\*Blynk IoT\*\* cho dự án \*\*ESP32 Motorcycle Monitor\*\*.



Blynk được dùng để:



\* Hiển thị tốc độ hiện tại.

\* Cài đặt tốc độ giới hạn.

\* Hiển thị tốc độ cao nhất.

\* Reset tốc độ cao nhất.

\* Hiển thị vị trí GPS.

\* Bật/tắt chế độ chống trộm.

\* Bật/tắt chức năng tìm xe.

\* Hiển thị trạng thái hệ thống.

\* Gửi cảnh báo quá tốc độ và cảnh báo nghi ngờ trộm.



\---



\## 2. Tạo Template



Trong Blynk Console, tạo một Template mới với thông tin gợi ý:



```text

Template Name: ESP32 Motorcycle Monitor

Hardware: ESP32

Connection Type: WiFi

```



Sau khi tạo Template, lấy các thông tin sau để đưa vào `secrets.h`:



```cpp

\#define BLYNK\_TEMPLATE\_ID "YourTemplateID"

\#define BLYNK\_TEMPLATE\_NAME "ESP32 Motorcycle Monitor"

\#define BLYNK\_AUTH\_TOKEN "YourAuthToken"

```



\---



\## 3. Datastreams



Tạo các Datastream dạng \*\*Virtual Pin\*\* như bảng dưới đây.



| Virtual Pin | Name            | Data Type |  Min | Max | Unit | Chức năng             |

| ----------- | --------------- | --------- | ---: | --: | ---- | --------------------- |

| V0          | Current Speed   | Integer   |    0 | 150 | km/h | Tốc độ hiện tại       |

| V1          | Speed Limit     | Integer   |    0 | 150 | km/h | Tốc độ giới hạn       |

| V2          | Guard Mode      | Integer   |    0 |   1 |      | Bật/tắt chống trộm    |

| V3          | Find Motorcycle | Integer   |    0 |   1 |      | Bật/tắt tìm xe        |

| V4          | Latitude        | Double    |  -90 |  90 |      | Vĩ độ                 |

| V5          | Longitude       | Double    | -180 | 180 |      | Kinh độ               |

| V6          | Map Link        | String    |      |     |      | Link Google Maps      |

| V7          | System Status   | String    |      |     |      | Trạng thái hệ thống   |

| V8          | Alert Status    | Integer   |    0 |   1 |      | Trạng thái cảnh báo   |

| V9          | Max Speed       | Integer   |    0 | 150 | km/h | Tốc độ cao nhất       |

| V10         | Reset Max Speed | Integer   |    0 |   1 |      | Reset tốc độ cao nhất |



\---



\## 4. Widget trên Blynk App



\### 4.1. Current Speed



Widget gợi ý:



```text

Gauge hoặc Value Display

```



Cấu hình:



```text

Datastream: V0 - Current Speed

Title: Tốc độ hiện tại

Unit: km/h

```



\---



\### 4.2. Speed Limit



Widget gợi ý:



```text

Slider hoặc Numeric Input

```



Cấu hình:



```text

Datastream: V1 - Speed Limit

Title: Tốc độ tối đa

Min: 0

Max: 150

Unit: km/h

```



Giá trị mặc định trong firmware:



```text

40 km/h

```



Khi ESP32 khởi động lại, firmware có thể ghi lại giá trị mặc định này lên app.



\---



\### 4.3. Guard Mode



Widget gợi ý:



```text

Switch

```



Cấu hình:



```text

Datastream: V2 - Guard Mode

Title: Chống trộm

OFF value: 0

ON value: 1

```



Ý nghĩa:



```text

OFF: Không kiểm tra MPU6050

ON: Bật chế độ phát hiện rung/lắc chống trộm

```



\---



\### 4.4. Find Motorcycle



Widget gợi ý:



```text

Switch

```



Cấu hình:



```text

Datastream: V3 - Find Motorcycle

Title: Tìm xe

OFF value: 0

ON value: 1

```



Ý nghĩa:



```text

ON: Bật buzzer để tìm xe

OFF: Tắt buzzer nếu không có cảnh báo khác

```



\---



\### 4.5. Latitude và Longitude



Widget gợi ý:



```text

Value Display

```



Cấu hình:



```text

Datastream: V4 - Latitude

Title: Vĩ độ

```



```text

Datastream: V5 - Longitude

Title: Kinh độ

```



\---



\### 4.6. Map Link



Widget gợi ý:



```text

Label hoặc Value Display

```



Cấu hình:



```text

Datastream: V6 - Map Link

Title: Vị trí xe

```



Dữ liệu có dạng:



```text

https://maps.google.com/?q=<latitude>,<longitude>

```



\---



\### 4.7. System Status



Widget gợi ý:



```text

Label

```



Cấu hình:



```text

Datastream: V7 - System Status

Title: Trạng thái

```



Các trạng thái có thể hiển thị:



| Trạng thái     | Ý nghĩa                    |

| -------------- | -------------------------- |

| Dang doi GPS   | GPS chưa có dữ liệu hợp lệ |

| Binh thuong    | Hệ thống bình thường       |

| Qua toc do     | Xe vượt tốc độ giới hạn    |

| Dang bao ve    | Chế độ chống trộm đang bật |

| Canh bao muc 1 | Phát hiện tác động lần 1   |

| Canh bao muc 2 | Phát hiện tác động lần 2   |

| Nghi ngo trom  | Phát hiện tác động mức 3   |



\---



\### 4.8. Alert Status



Widget gợi ý:



```text

LED hoặc Value Display

```



Cấu hình:



```text

Datastream: V8 - Alert Status

Title: Cảnh báo

OFF value: 0

ON value: 1

```



Ý nghĩa:



```text

0: Không có cảnh báo nghiêm trọng

1: Có cảnh báo

```



\---



\### 4.9. Max Speed



Widget gợi ý:



```text

Value Display hoặc Gauge

```



Cấu hình:



```text

Datastream: V9 - Max Speed

Title: Tốc độ cao nhất

Unit: km/h

```



Ý nghĩa:



```text

Hiển thị tốc độ lớn nhất đã ghi nhận từ khi ESP32 khởi động.

```



\---



\### 4.10. Reset Max Speed



Widget gợi ý:



```text

Button

```



Cấu hình:



```text

Datastream: V10 - Reset Max Speed

Title: Xóa tốc độ cao nhất

Mode: Push

OFF value: 0

ON value: 1

OFF label: Chờ

ON label: Xóa

```



Ý nghĩa:



```text

Khi bấm nút, ESP32 đặt Max Speed về 0.

```



\---



\## 5. Events



Tạo các Event trong Blynk Console để gửi notification.



\### 5.1. Overspeed Alert



```text

Event code: overspeed\_alert

Event name: Overspeed Alert

```



Điều kiện kích hoạt trong firmware:



```text

currentSpeedKmh > speedLimitKmh

```



Nội dung thông báo:



```text

Xe vuot qua toc do. Toc do: <currentSpeed> km/h. Gioi han: <speedLimit> km/h.

```



Thông báo quá tốc độ \*\*không gửi kèm link Google Maps\*\*.



\---



\### 5.2. Theft Alert



```text

Event code: theft\_alert

Event name: Theft Alert

```



Điều kiện kích hoạt trong firmware:



```text

Guard Mode = ON

MPU6050 phát hiện tác động mức 3

```



Nội dung thông báo:



```text

Canh bao nghi ngo trom. Vi tri: <Google Maps link>

```



Thông báo nghi ngờ trộm \*\*có gửi kèm link Google Maps\*\*.



\---



\## 6. Quy ước hoạt động



\### 6.1. Khi ESP32 khởi động



Firmware sẽ:



\* Kết nối WiFi.

\* Kết nối Blynk.

\* Khởi tạo GPS.

\* Khởi tạo MPU6050.

\* Cập nhật tốc độ, vị trí và trạng thái lên app.



Có thể cấu hình mặc định:



```text

Speed Limit = 40 km/h

Guard Mode = OFF

Find Motorcycle = OFF

```



\---



\### 6.2. Khi GPS chưa có dữ liệu



App có thể hiển thị:



```text

Dang doi GPS

```



Tốc độ hiện tại có thể là:



```text

0 km/h

```



Link bản đồ có thể hiển thị:



```text

Dang doi GPS...

```



\---



\### 6.3. Khi quá tốc độ



Hệ thống sẽ:



\* Bật buzzer.

\* Cập nhật trạng thái `Qua toc do`.

\* Gửi event `overspeed\_alert`.



\---



\### 6.4. Khi bật Guard Mode



Hệ thống sẽ:



\* Reset trạng thái chống trộm.

\* Lấy mốc MPU6050 tại trạng thái đứng yên.

\* Bắt đầu phát hiện rung/lắc bất thường.



Logic cảnh báo:



```text

Lần 1: bíp 1 lần

Lần 2: bíp 2 lần

Lần 3: còi mạnh 5 giây + gửi theft\_alert

```



\---



\## 7. Lưu ý chống Blynk bị Offline



Không nên gửi dữ liệu lên Blynk quá nhanh.



Trong firmware, nên tách chu kỳ gửi:



| Dữ liệu            |                       Chu kỳ gợi ý |

| ------------------ | ---------------------------------: |

| Tốc độ hiện tại    |                500 ms hoặc 1000 ms |

| Trạng thái         | 500 ms, nhưng chỉ gửi khi thay đổi |

| Vị trí và link map |                           30000 ms |

| Debug Serial       |                Không gửi lên Blynk |



Nguyên tắc:



```text

Dữ liệu thay đổi nhanh thì gửi nhanh hơn.

Dữ liệu dài hoặc ít thay đổi thì gửi chậm hơn.

Trạng thái chỉ gửi khi thay đổi.

```



\---



\## 8. File cấu hình liên quan



Các Virtual Pin được khai báo trong:



```text

firmware/esp32\_main/config.h

```



Thông tin WiFi và Blynk token thật được đặt trong:



```text

firmware/esp32\_main/secrets.h

```



File `secrets.h` không được commit lên GitHub.



