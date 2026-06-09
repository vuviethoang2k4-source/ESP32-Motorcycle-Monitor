\# Test Plan



\## 1. Mục đích



File này mô tả kế hoạch kiểm thử cho dự án \*\*ESP32 Motorcycle Monitor\*\*.



Mục tiêu kiểm thử:



\* Đảm bảo ESP32 kết nối WiFi và Blynk ổn định.

\* Đảm bảo GPS NEO-8M đọc được tốc độ và vị trí thật.

\* Đảm bảo MPU6050 phát hiện rung/lắc đúng.

\* Đảm bảo cảnh báo quá tốc độ hoạt động.

\* Đảm bảo chống trộm 3 cấp hoạt động đúng.

\* Đảm bảo buzzer và LED phản ứng đúng với từng trạng thái.

\* Đảm bảo Blynk App hiển thị đúng dữ liệu.



\---



\## 2. Môi trường kiểm thử



| Thành phần              | Thông tin                    |

| ----------------------- | ---------------------------- |

| Board                   | ESP32 DevKit V1              |

| GPS                     | NEO-8M                       |

| Cảm biến chuyển động    | MPU6050                      |

| App                     | Blynk IoT                    |

| IDE                     | Arduino IDE                  |

| Baudrate Serial Monitor | 115200                       |

| Kết nối mạng            | WiFi hoặc hotspot điện thoại |



\---



\## 3. Kiểm thử nguồn cấp



\### TC-01: Cấp nguồn bằng USB



| Mục              | Nội dung                                             |

| ---------------- | ---------------------------------------------------- |

| Mục tiêu         | Kiểm tra ESP32 chạy ổn định khi cấp nguồn qua USB    |

| Cách test        | Cắm ESP32 vào máy tính hoặc củ sạc điện thoại        |

| Kết quả mong đợi | ESP32 khởi động, Serial Monitor có log, Blynk Online |



\---



\### TC-02: Cấp nguồn bằng pin 18650 và mạch tăng áp 5V



| Mục              | Nội dung                                                |

| ---------------- | ------------------------------------------------------- |

| Mục tiêu         | Kiểm tra hệ thống chạy bằng nguồn pin                   |

| Cách test        | Pin 18650 → mạch sạc/tăng áp 5V → ESP32 5V/VIN hoặc USB |

| Kết quả mong đợi | ESP32 chạy ổn định, không reset, Blynk Online           |



\---



\## 4. Kiểm thử kết nối Blynk



\### TC-03: ESP32 kết nối Blynk



| Mục              | Nội dung                                |

| ---------------- | --------------------------------------- |

| Mục tiêu         | Kiểm tra ESP32 kết nối được Blynk Cloud |

| Cách test        | Upload firmware chính, mở Blynk App     |

| Kết quả mong đợi | Device hiển thị Online                  |



\---



\### TC-04: Giá trị mặc định sau khi khởi động



| Mục              | Nội dung                                                        |

| ---------------- | --------------------------------------------------------------- |

| Mục tiêu         | Kiểm tra giá trị mặc định khi ESP32 khởi động lại               |

| Cách test        | Đặt Speed Limit khác 40, bật Guard Mode, sau đó reset ESP32     |

| Kết quả mong đợi | Speed Limit trở về 40 km/h, Guard Mode OFF, Find Motorcycle OFF |



\---



\## 5. Kiểm thử GPS



\### TC-05: GPS có dữ liệu hợp lệ



| Mục              | Nội dung                                                    |

| ---------------- | ----------------------------------------------------------- |

| Mục tiêu         | Kiểm tra GPS bắt được tín hiệu                              |

| Cách test        | Đặt anten GPS gần cửa sổ hoặc ngoài trời                    |

| Kết quả mong đợi | Serial Monitor hiển thị `GPS valid: YES`, có vĩ độ, kinh độ |



\---



\### TC-06: Hiển thị vị trí trên Blynk



| Mục              | Nội dung                                                             |

| ---------------- | -------------------------------------------------------------------- |

| Mục tiêu         | Kiểm tra tọa độ GPS gửi lên Blynk                                    |

| Cách test        | Quan sát V4, V5, V6 trên app                                         |

| Kết quả mong đợi | V4 hiển thị vĩ độ, V5 hiển thị kinh độ, V6 hiển thị link Google Maps |



\---



\### TC-07: Hiển thị tốc độ hiện tại



| Mục              | Nội dung                               |

| ---------------- | -------------------------------------- |

| Mục tiêu         | Kiểm tra tốc độ hiện tại               |

| Cách test        | Di chuyển thiết bị hoặc thử ngoài trời |

| Kết quả mong đợi | V0 cập nhật tốc độ hiện tại theo GPS   |



\---



\## 6. Kiểm thử quá tốc độ



\### TC-08: Không quá tốc độ



| Mục              | Nội dung                                                   |

| ---------------- | ---------------------------------------------------------- |

| Mục tiêu         | Kiểm tra trạng thái bình thường khi tốc độ dưới giới hạn   |

| Cách test        | Đặt Speed Limit lớn hơn tốc độ hiện tại                    |

| Kết quả mong đợi | Trạng thái là `Binh thuong` hoặc `Dang bao ve`, buzzer tắt |



\---



\### TC-09: Quá tốc độ



| Mục              | Nội dung                                                                  |

| ---------------- | ------------------------------------------------------------------------- |

| Mục tiêu         | Kiểm tra cảnh báo quá tốc độ                                              |

| Cách test        | Đặt Speed Limit thấp hơn tốc độ hiện tại                                  |

| Kết quả mong đợi | V7 hiển thị `Qua toc do`, V8 = 1, buzzer kêu, Blynk gửi `overspeed\_alert` |



\---



\### TC-10: Hết quá tốc độ



| Mục              | Nội dung                                                         |

| ---------------- | ---------------------------------------------------------------- |

| Mục tiêu         | Kiểm tra hệ thống trở lại bình thường                            |

| Cách test        | Tăng Speed Limit cao hơn tốc độ hiện tại                         |

| Kết quả mong đợi | Buzzer tắt nếu không có cảnh báo khác, trạng thái về bình thường |



\---



\## 7. Kiểm thử Max Speed



\### TC-11: Cập nhật Max Speed



| Mục              | Nội dung                                   |

| ---------------- | ------------------------------------------ |

| Mục tiêu         | Kiểm tra ghi nhận tốc độ cao nhất          |

| Cách test        | Cho tốc độ hiện tại thay đổi tăng/giảm     |

| Kết quả mong đợi | V9 giữ giá trị tốc độ lớn nhất đã ghi nhận |



Ví dụ:



```text

Current Speed = 5 km/h  → Max Speed = 5 km/h

Current Speed = 12 km/h → Max Speed = 12 km/h

Current Speed = 6 km/h  → Max Speed vẫn = 12 km/h

```



\---



\### TC-12: Reset Max Speed



| Mục              | Nội dung                           |

| ---------------- | ---------------------------------- |

| Mục tiêu         | Kiểm tra nút reset tốc độ cao nhất |

| Cách test        | Bấm nút V10 trên Blynk App         |

| Kết quả mong đợi | V9 trở về 0 km/h                   |



\---



\## 8. Kiểm thử Find Motorcycle



\### TC-13: Bật tìm xe



| Mục              | Nội dung                  |

| ---------------- | ------------------------- |

| Mục tiêu         | Kiểm tra chức năng tìm xe |

| Cách test        | Bật V3 - Find Motorcycle  |

| Kết quả mong đợi | Buzzer kêu, LED sáng      |



\---



\### TC-14: Tắt tìm xe



| Mục              | Nội dung                              |

| ---------------- | ------------------------------------- |

| Mục tiêu         | Kiểm tra tắt chức năng tìm xe         |

| Cách test        | Tắt V3 - Find Motorcycle              |

| Kết quả mong đợi | Buzzer tắt nếu không có cảnh báo khác |



\---



\## 9. Kiểm thử MPU6050



\### TC-15: MPU6050 hoạt động



| Mục              | Nội dung                      |

| ---------------- | ----------------------------- |

| Mục tiêu         | Kiểm tra ESP32 nhận MPU6050   |

| Cách test        | Mở Serial Monitor             |

| Kết quả mong đợi | Log hiển thị `MPU ready: YES` |



\---



\### TC-16: Guard Mode OFF



| Mục              | Nội dung                                               |

| ---------------- | ------------------------------------------------------ |

| Mục tiêu         | Kiểm tra chống trộm không hoạt động khi Guard Mode tắt |

| Cách test        | Tắt V2, lắc MPU6050                                    |

| Kết quả mong đợi | Không đếm cảnh báo chống trộm, không gửi `theft\_alert` |



\---



\### TC-17: Guard Mode ON



| Mục              | Nội dung                                                     |

| ---------------- | ------------------------------------------------------------ |

| Mục tiêu         | Kiểm tra hệ thống vào chế độ bảo vệ                          |

| Cách test        | Bật V2 - Guard Mode                                          |

| Kết quả mong đợi | V7 hiển thị `Dang bao ve`, hệ thống bắt đầu giám sát MPU6050 |



\---



\## 10. Kiểm thử chống trộm 3 cấp



\### TC-18: Tác động lần 1



| Mục              | Nội dung                                        |

| ---------------- | ----------------------------------------------- |

| Mục tiêu         | Kiểm tra cảnh báo mức 1                         |

| Cách test        | Bật Guard Mode, lắc/gõ nhẹ đủ vượt ngưỡng lần 1 |

| Kết quả mong đợi | Buzzer bíp 1 lần, V7 hiển thị `Canh bao muc 1`  |



\---



\### TC-19: Tác động lần 2



| Mục              | Nội dung                                       |

| ---------------- | ---------------------------------------------- |

| Mục tiêu         | Kiểm tra cảnh báo mức 2                        |

| Cách test        | Sau lần 1, chờ qua cooldown rồi tác động lần 2 |

| Kết quả mong đợi | Buzzer bíp 2 lần, V7 hiển thị `Canh bao muc 2` |



\---



\### TC-20: Tác động lần 3



| Mục              | Nội dung                                                                                    |

| ---------------- | ------------------------------------------------------------------------------------------- |

| Mục tiêu         | Kiểm tra cảnh báo nghi ngờ trộm                                                             |

| Cách test        | Sau lần 2, chờ qua cooldown rồi tác động lần 3                                              |

| Kết quả mong đợi | Buzzer kêu mạnh 5 giây, V7 hiển thị `Nghi ngo trom`, V8 = 1, gửi `theft\_alert` kèm link map |



\---



\### TC-21: Reset mức cảnh báo chống trộm



| Mục              | Nội dung                                                                   |

| ---------------- | -------------------------------------------------------------------------- |

| Mục tiêu         | Kiểm tra reset số lần tác động                                             |

| Cách test        | Sau cảnh báo mức 1 hoặc 2, không tác động thêm trong thời gian reset       |

| Kết quả mong đợi | Hệ thống reset tamper count về 0, lần tác động tiếp theo tính lại từ mức 1 |



\---



\## 11. Kiểm thử ưu tiên trạng thái



\### TC-22: Quá tốc độ và Guard Mode cùng bật



| Mục              | Nội dung                                                                                                                   |

| ---------------- | -------------------------------------------------------------------------------------------------------------------------- |

| Mục tiêu         | Kiểm tra ưu tiên trạng thái                                                                                                |

| Cách test        | Bật Guard Mode và tạo điều kiện quá tốc độ                                                                                 |

| Kết quả mong đợi | Nếu chưa có cảnh báo trộm, trạng thái có thể hiển thị `Qua toc do` hoặc `Dang bao ve` tùy điều kiện ưu tiên trong firmware |



\---



\### TC-23: Nghi ngờ trộm và quá tốc độ cùng lúc



| Mục              | Nội dung                                                        |

| ---------------- | --------------------------------------------------------------- |

| Mục tiêu         | Kiểm tra cảnh báo trộm được ưu tiên                             |

| Cách test        | Tạo cảnh báo chống trộm mức 3 khi hệ thống cũng đang quá tốc độ |

| Kết quả mong đợi | V7 ưu tiên hiển thị `Nghi ngo trom`                             |



\---



\## 12. Kiểm thử độ ổn định



\### TC-24: Chạy liên tục



| Mục              | Nội dung                                                             |

| ---------------- | -------------------------------------------------------------------- |

| Mục tiêu         | Kiểm tra hệ thống chạy ổn định                                       |

| Cách test        | Cấp nguồn cho hệ thống chạy liên tục ít nhất 30 phút                 |

| Kết quả mong đợi | ESP32 không reset, Blynk không offline, GPS và MPU6050 vẫn hoạt động |



\---



\### TC-25: Blynk không bị flood



| Mục              | Nội dung                                                               |

| ---------------- | ---------------------------------------------------------------------- |

| Mục tiêu         | Kiểm tra hệ thống không gửi Blynk quá dày                              |

| Cách test        | Chạy firmware chính, quan sát trạng thái online                        |

| Kết quả mong đợi | Thiết bị duy trì Online, không bị mất kết nối do gửi dữ liệu quá nhiều |



\---



\## 13. Kết luận kiểm thử



Hệ thống được xem là đạt yêu cầu nếu:



\* Blynk Online ổn định.

\* GPS đọc được tốc độ và vị trí.

\* Buzzer hoạt động đúng.

\* Quá tốc độ có cảnh báo.

\* Max Speed cập nhật đúng.

\* Reset Max Speed hoạt động.

\* MPU6050 phát hiện rung/lắc đúng.

\* Chống trộm 3 cấp hoạt động đúng.

\* Cảnh báo nghi ngờ trộm gửi kèm link vị trí.



