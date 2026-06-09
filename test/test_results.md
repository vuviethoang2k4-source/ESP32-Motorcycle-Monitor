\# Test Results



\## 1. Mục đích



File này ghi lại kết quả kiểm thử cho dự án \*\*ESP32 Motorcycle Monitor\*\*.



Các kết quả bên dưới được dùng để xác nhận các chức năng chính của hệ thống:



\* Kết nối Blynk.

\* Đọc GPS.

\* Cảnh báo quá tốc độ.

\* Hiển thị tốc độ cao nhất.

\* Reset tốc độ cao nhất.

\* Chức năng tìm xe.

\* MPU6050 chống trộm 3 cấp.

\* Buzzer và LED cảnh báo.



\---



\## 2. Thông tin hệ thống kiểm thử



| Thành phần              | Thông tin           |

| ----------------------- | ------------------- |

| Board                   | ESP32 DevKit V1     |

| GPS                     | NEO-8M              |

| Cảm biến chuyển động    | MPU6050             |

| Cảnh báo tại thiết bị   | Active Buzzer + LED |

| App                     | Blynk IoT           |

| Kết nối mạng            | WiFi / Hotspot      |

| IDE                     | Arduino IDE         |

| Baudrate Serial Monitor | 115200              |



\---



\## 3. Kết quả kiểm thử tổng hợp



| Test Case | Nội dung kiểm thử                           | Kết quả |

| --------- | ------------------------------------------- | ------- |

| TC-01     | Cấp nguồn bằng USB                          | Pass    |

| TC-02     | Cấp nguồn bằng pin 18650 và mạch tăng áp 5V | Pending |

| TC-03     | ESP32 kết nối Blynk                         | Pass    |

| TC-04     | Giá trị mặc định sau khi khởi động          | Pass    |

| TC-05     | GPS có dữ liệu hợp lệ                       | Pass    |

| TC-06     | Hiển thị vị trí trên Blynk                  | Pass    |

| TC-07     | Hiển thị tốc độ hiện tại                    | Pass    |

| TC-08     | Không quá tốc độ                            | Pass    |

| TC-09     | Quá tốc độ                                  | Pass    |

| TC-10     | Hết quá tốc độ                              | Pass    |

| TC-11     | Cập nhật Max Speed                          | Pass    |

| TC-12     | Reset Max Speed                             | Pass    |

| TC-13     | Bật tìm xe                                  | Pass    |

| TC-14     | Tắt tìm xe                                  | Pass    |

| TC-15     | MPU6050 hoạt động                           | Pass    |

| TC-16     | Guard Mode OFF                              | Pass    |

| TC-17     | Guard Mode ON                               | Pass    |

| TC-18     | Tác động lần 1                              | Pass    |

| TC-19     | Tác động lần 2                              | Pass    |

| TC-20     | Tác động lần 3                              | Pass    |

| TC-21     | Reset mức cảnh báo chống trộm               | Pass    |

| TC-22     | Quá tốc độ và Guard Mode cùng bật           | Pass    |

| TC-23     | Nghi ngờ trộm và quá tốc độ cùng lúc        | Pass    |

| TC-24     | Chạy liên tục                               | Pending |

| TC-25     | Blynk không bị flood                        | Pass    |



\---



\## 4. Kết quả kiểm thử chi tiết



\### TC-01: Cấp nguồn bằng USB



| Mục              | Kết quả                                  |

| ---------------- | ---------------------------------------- |

| Cách test        | Cắm ESP32 DevKit V1 vào máy tính qua USB |

| Kết quả quan sát | ESP32 khởi động, Serial Monitor có log   |

| Kết luận         | Pass                                     |



\---



\### TC-02: Cấp nguồn bằng pin 18650 và mạch tăng áp 5V



| Mục              | Kết quả                                           |

| ---------------- | ------------------------------------------------- |

| Cách test        | Pin 18650 → mạch sạc/tăng áp 5V → ESP32           |

| Kết quả quan sát | Chưa kiểm thử hoàn chỉnh với khối nguồn cuối cùng |

| Kết luận         | Pending                                           |



\---



\### TC-03: ESP32 kết nối Blynk



| Mục              | Kết quả                               |

| ---------------- | ------------------------------------- |

| Cách test        | Upload firmware chính và mở Blynk App |

| Kết quả quan sát | Thiết bị hiển thị Online trên Blynk   |

| Kết luận         | Pass                                  |



\---



\### TC-04: Giá trị mặc định sau khi khởi động



| Mục              | Kết quả                                                |

| ---------------- | ------------------------------------------------------ |

| Cách test        | Thay đổi Speed Limit và Guard Mode, sau đó reset ESP32 |

| Kết quả mong đợi | Speed Limit = 40 km/h, Guard Mode OFF                  |

| Kết quả quan sát | Hệ thống trở về giá trị mặc định                       |

| Kết luận         | Pass                                                   |



\---



\### TC-05: GPS có dữ liệu hợp lệ



| Mục              | Kết quả                                  |

| ---------------- | ---------------------------------------- |

| Cách test        | Đặt GPS gần cửa sổ hoặc vị trí thoáng    |

| Kết quả quan sát | Serial Monitor hiển thị `GPS valid: YES` |

| Kết luận         | Pass                                     |



Ghi chú:



```text

GPS có thể mất vài phút để bắt tín hiệu khi mới khởi động.

Thời gian bắt GPS phụ thuộc vị trí đặt anten và điều kiện môi trường.

```



\---



\### TC-06: Hiển thị vị trí trên Blynk



| Mục              | Kết quả                                           |

| ---------------- | ------------------------------------------------- |

| Cách test        | Quan sát Latitude, Longitude và Map Link trên app |

| Kết quả quan sát | App hiển thị vĩ độ, kinh độ và link Google Maps   |

| Kết luận         | Pass                                              |



\---



\### TC-07: Hiển thị tốc độ hiện tại



| Mục              | Kết quả                               |

| ---------------- | ------------------------------------- |

| Cách test        | Quan sát tốc độ GPS trên Blynk App    |

| Kết quả quan sát | Tốc độ hiện tại được cập nhật lên app |

| Kết luận         | Pass                                  |



Ghi chú:



```text

Tốc độ trên app có thể có độ trễ do GPS và Blynk Cloud.

Còi cảnh báo tại ESP32 phản ứng nhanh hơn app.

```



\---



\### TC-08: Không quá tốc độ



| Mục              | Kết quả                                     |

| ---------------- | ------------------------------------------- |

| Cách test        | Đặt Speed Limit lớn hơn tốc độ hiện tại     |

| Kết quả quan sát | Buzzer tắt, trạng thái không báo quá tốc độ |

| Kết luận         | Pass                                        |



\---



\### TC-09: Quá tốc độ



| Mục              | Kết quả                                  |

| ---------------- | ---------------------------------------- |

| Cách test        | Đặt Speed Limit thấp hơn tốc độ hiện tại |

| Kết quả quan sát | Blynk báo quá tốc độ, buzzer kêu         |

| Kết luận         | Pass                                     |



Thông báo sử dụng event:



```text

overspeed\_alert

```



\---



\### TC-10: Hết quá tốc độ



| Mục              | Kết quả                                                        |

| ---------------- | -------------------------------------------------------------- |

| Cách test        | Tăng Speed Limit cao hơn tốc độ hiện tại                       |

| Kết quả quan sát | Cảnh báo quá tốc độ tắt, buzzer tắt nếu không có cảnh báo khác |

| Kết luận         | Pass                                                           |



\---



\### TC-11: Cập nhật Max Speed



| Mục              | Kết quả                                    |

| ---------------- | ------------------------------------------ |

| Cách test        | Thay đổi tốc độ hiện tại                   |

| Kết quả quan sát | V9 giữ giá trị tốc độ lớn nhất đã ghi nhận |

| Kết luận         | Pass                                       |



Ví dụ:



```text

Current Speed tăng → Max Speed tăng theo.

Current Speed giảm → Max Speed giữ nguyên.

```



\---



\### TC-12: Reset Max Speed



| Mục              | Kết quả                    |

| ---------------- | -------------------------- |

| Cách test        | Bấm nút V10 trên Blynk App |

| Kết quả quan sát | V9 trở về 0 km/h           |

| Kết luận         | Pass                       |



Ghi chú:



```text

Nếu reset khi xe đang di chuyển, Max Speed có thể về 0 rồi tăng ngay lên tốc độ hiện tại.

```



\---



\### TC-13: Bật tìm xe



| Mục              | Kết quả                  |

| ---------------- | ------------------------ |

| Cách test        | Bật V3 - Find Motorcycle |

| Kết quả quan sát | Buzzer kêu, LED sáng     |

| Kết luận         | Pass                     |



\---



\### TC-14: Tắt tìm xe



| Mục              | Kết quả                               |

| ---------------- | ------------------------------------- |

| Cách test        | Tắt V3 - Find Motorcycle              |

| Kết quả quan sát | Buzzer tắt nếu không có cảnh báo khác |

| Kết luận         | Pass                                  |



\---



\### TC-15: MPU6050 hoạt động



| Mục              | Kết quả                                  |

| ---------------- | ---------------------------------------- |

| Cách test        | Mở Serial Monitor                        |

| Kết quả quan sát | Serial Monitor hiển thị `MPU ready: YES` |

| Kết luận         | Pass                                     |



\---



\### TC-16: Guard Mode OFF



| Mục              | Kết quả                             |

| ---------------- | ----------------------------------- |

| Cách test        | Tắt Guard Mode rồi lắc MPU6050      |

| Kết quả quan sát | Không kích hoạt cảnh báo chống trộm |

| Kết luận         | Pass                                |



\---



\### TC-17: Guard Mode ON



| Mục              | Kết quả                                |

| ---------------- | -------------------------------------- |

| Cách test        | Bật Guard Mode trên app                |

| Kết quả quan sát | Hệ thống chuyển sang trạng thái bảo vệ |

| Kết luận         | Pass                                   |



\---



\### TC-18: Tác động lần 1



| Mục              | Kết quả                            |

| ---------------- | ---------------------------------- |

| Cách test        | Bật Guard Mode, tạo tác động lần 1 |

| Kết quả quan sát | Buzzer bíp 1 lần, trạng thái mức 1 |

| Kết luận         | Pass                               |



\---



\### TC-19: Tác động lần 2



| Mục              | Kết quả                                        |

| ---------------- | ---------------------------------------------- |

| Cách test        | Sau lần 1, chờ cooldown rồi tạo tác động lần 2 |

| Kết quả quan sát | Buzzer bíp 2 lần, trạng thái mức 2             |

| Kết luận         | Pass                                           |



\---



\### TC-20: Tác động lần 3



| Mục              | Kết quả                                         |

| ---------------- | ----------------------------------------------- |

| Cách test        | Sau lần 2, chờ cooldown rồi tạo tác động lần 3  |

| Kết quả quan sát | Buzzer kêu mạnh 5 giây, Blynk gửi `theft\_alert` |

| Kết luận         | Pass                                            |



Thông báo sử dụng event:



```text

theft\_alert

```



Nội dung cảnh báo có kèm link Google Maps.



\---



\### TC-21: Reset mức cảnh báo chống trộm



| Mục              | Kết quả                                                              |

| ---------------- | -------------------------------------------------------------------- |

| Cách test        | Sau cảnh báo mức 1 hoặc 2, không tác động thêm trong thời gian reset |

| Kết quả quan sát | Tamper count trở về 0, lần tác động sau tính lại từ mức 1            |

| Kết luận         | Pass                                                                 |



\---



\### TC-22: Quá tốc độ và Guard Mode cùng bật



| Mục              | Kết quả                                                  |

| ---------------- | -------------------------------------------------------- |

| Cách test        | Bật Guard Mode và tạo điều kiện quá tốc độ               |

| Kết quả quan sát | Hệ thống vẫn cảnh báo quá tốc độ, Guard Mode vẫn duy trì |

| Kết luận         | Pass                                                     |



\---



\### TC-23: Nghi ngờ trộm và quá tốc độ cùng lúc



| Mục              | Kết quả                                                          |

| ---------------- | ---------------------------------------------------------------- |

| Cách test        | Tạo cảnh báo chống trộm mức 3 khi hệ thống đang có cảnh báo khác |

| Kết quả quan sát | Trạng thái `Nghi ngo trom` được ưu tiên                          |

| Kết luận         | Pass                                                             |



\---



\### TC-24: Chạy liên tục



| Mục              | Kết quả                                    |

| ---------------- | ------------------------------------------ |

| Cách test        | Cho hệ thống chạy liên tục ít nhất 30 phút |

| Kết quả quan sát | Chưa kiểm thử đầy đủ trong thời gian dài   |

| Kết luận         | Pending                                    |



\---



\### TC-25: Blynk không bị flood



| Mục              | Kết quả                                                   |

| ---------------- | --------------------------------------------------------- |

| Cách test        | Chạy firmware chính và quan sát trạng thái online         |

| Kết quả quan sát | Blynk duy trì Online khi chu kỳ gửi dữ liệu được giới hạn |

| Kết luận         | Pass                                                      |



Ghi chú:



```text

Tốc độ và Max Speed chỉ gửi khi giá trị thay đổi.

Vị trí và link map được gửi chậm hơn để tránh làm Blynk bị quá tải.

```



\---



\## 5. Vấn đề đã gặp và cách xử lý



| Vấn đề                   | Nguyên nhân                          | Cách xử lý                                          |

| ------------------------ | ------------------------------------ | --------------------------------------------------- |

| Blynk bị Offline         | Gửi dữ liệu lên Blynk quá nhanh      | Giảm tần suất gửi tốc độ, gửi vị trí chậm hơn       |

| GPS bắt tín hiệu lâu     | Anten GPS bị che hoặc GPS cold start | Đặt GPS gần cửa sổ/ngoài trời, anten hướng lên trời |

| MPU6050 không nhận       | Sai dây SDA/SCL hoặc địa chỉ I2C     | Dùng I2C Scanner, kiểm tra địa chỉ 0x68/0x69        |

| Buzzer không kêu         | Chưa xác định loại buzzer            | Test riêng GPIO25, xác nhận active HIGH             |

| Lỗi compile redefinition | Paste code mới nối tiếp code cũ      | Xóa toàn bộ code cũ trước khi paste code mới        |



\---



\## 6. Kết luận



Hệ thống đã kiểm thử thành công các chức năng chính:



\* Kết nối Blynk.

\* Đọc GPS thật.

\* Hiển thị tốc độ và vị trí.

\* Cảnh báo quá tốc độ.

\* Ghi nhận và reset tốc độ cao nhất.

\* Tìm xe bằng buzzer.

\* MPU6050 phát hiện rung/lắc.

\* Chống trộm 3 cấp.

\* Gửi cảnh báo nghi ngờ trộm kèm vị trí.



Các phần cần kiểm thử thêm:



\* Chạy bằng khối nguồn pin 18650 hoàn chỉnh.

\* Chạy ổn định trong thời gian dài.

\* Đo thời lượng pin thực tế.

\* Kiểm thử khi lắp vào hộp hoặc gắn lên xe thật.



