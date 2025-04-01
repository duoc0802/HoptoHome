# HoptoHome-Phạm Xuân Được-24020099
- Video game demo: https://www.youtube.com/watch?v=uuX4BwD4XPE

# Giới thiệu game:
"Hop to Home" là một trò chơi giải đố vui nhộn với phong cách đồ họa dễ thương, nơi người chơi điều khiển một chú thỏ tìm đường về nhà. Trò chơi mang đến trải nghiệm thú vị cho mọi lứa tuổi nhờ sự kết hợp giữa yếu tố pathfinding (tìm đường) và puzzle (giải đố).
* Cách tải game
* Logic Game
* Bắt đầu game
* Chiến thắng
* Thất Bại
* Các thành phần của game
* Tính năng phát triển game
* Source code game
# Cách tải game (khoảng 125MB):
- ẤN chọn ô "Code" trong màn hình chính của Github

![image](https://github.com/user-attachments/assets/5e3c98a2-1ab5-4a61-8bc7-6210b4d158da)
- Chọn "Download ZIP"

![image](https://github.com/user-attachments/assets/bf0a7045-a337-4079-b17f-d3b2de57aae8)

# Logic game:
- Sử dụng chuột giúp chú thỏ di chuyển từ vị trí ban đầu đến ngôi nhà bằng cách vẽ một đường đi liên tục trên lưới ô vuông, đồng thời tránh các chướng ngại vật và đảm bảo đi qua tất cả các ô trống.
# Bắt đầu game:
![Screenshot 2025-03-30 225829](https://github.com/user-attachments/assets/04d09835-37ec-4926-b584-fe8ee4dce5e9)

-Menu hiện lên có tên và 3 nút chức năng (Start: Bắt đầu; Music On/OFF: bật tắt nhạc; Guide: hướng dẫn chơi)
+ Khi ấn vào Guide, bảng hướng dẫn thông báo chơi sẽ hiện lên, người chơi đọc và hiểu luật

![image](https://github.com/user-attachments/assets/f16aeba8-fe66-4a26-bc65-96dbd2969802)
+ Khi ấn vào Start, giao diện chính trò chơi hiện lên, bên trái màn hình là đồng hồ đếm ngược 30s, điểm số và nút chức năng Pause để tạm dừng/ tiếp tục trò chơi

![image](https://github.com/user-attachments/assets/cabbd6e5-7518-49a2-8423-40874a7f0561)
# Chiến thắng:
- Khi đường đi hợp lệ (phủ kín tất cả ô trống và kết nối từ thỏ đến nhà), level hoàn thành, người chơi được cộng điểm (10 điểm mỗi level) và có thông báo chuyển sang level tiếp theo.

![image](https://github.com/user-attachments/assets/feeb5f24-91ab-424e-8fb7-0926941e06d9)
- Khi bạn vượt qua level cuối, bạn sẽ có thông báo chiến thắng

![Screenshot 2025-03-30 233334](https://github.com/user-attachments/assets/4bcf6b8d-4f4c-496a-89fb-ad69e00ab383)
# Thất bại:
- Nếu hết thời gian hoặc không thể tìm được đường đi hợp lệ, trò chơi kết thúc và quay về menu chính

![image](https://github.com/user-attachments/assets/753e46d6-219c-4756-be4f-913a7d109757)
# Các thành phần trong game:
- Nút Start: Bắt đầu trò chơi

![image](https://github.com/user-attachments/assets/3c104b01-4b9f-497b-97b2-6b97adf17961)
- Nút Music ON/OFF: Bật, tắt nhạc

![image](https://github.com/user-attachments/assets/89ba2c7d-1747-45f8-aad2-aa58bc520de2)
-Nút Guide: Bật bảng hướng dẫn trò chơi

![image](https://github.com/user-attachments/assets/dfab92e0-901d-4e3c-a1cf-400fd9ad86a3)
- Bảng hướng dẫn:

![image](https://github.com/user-attachments/assets/7ad7abc8-370d-45cf-8e8d-8e1f62d919ec)
- Đồng hồ đếm ngược thời gian: Đếm ngược 30s/ 1 level

![image](https://github.com/user-attachments/assets/f3c8d272-9cfa-4509-be12-f9a2ffedb647)
- Bảng đếm điểm số: tăng 10 điểm khi vượt qua 1 level

![image](https://github.com/user-attachments/assets/ff631a18-9a2e-46f6-be1e-1fe7f967e968)
- Nút Pause: Tạm dừng, tiếp tục trò chơi

![image](https://github.com/user-attachments/assets/f97f4a50-0d42-4758-a577-2e0fb16fc79c)
- Nhân vật thỏ:

![image](https://github.com/user-attachments/assets/8f89eb55-11c9-4bd5-bb57-7cdbd3136684)
- Đích đến, nhà:

![image](https://github.com/user-attachments/assets/64bd5ab6-3d07-48b5-a283-330f0b302952)
- Đường đi: được vẽ liền liên tục và bôi kín các ô vuông không có chướng ngại vật:

![image](https://github.com/user-attachments/assets/dce8c409-993b-44d4-82e5-8401dda68436)
# Tính năng phát triển game:
- Đồ họa dễ thương: Hình ảnh thỏ, nhà, và các chướng ngại vật được thiết kế sinh động, thu hút người chơi, đặc biệt là trẻ em. 
- Gameplay đơn giản nhưng thú vị: Dễ học (chỉ cần kéo chuột), nhưng khó để thành thạo do yêu cầu phủ kín lưới và tránh chướng ngại vật. 
- Nhiều level: Với nhiều level có độ khó tăng dần thông qua kích thước lưới lớn hơn và số lượng chướng ngại vật nhiều hơn, game đảm bảo người chơi có nội dung phong phú để khám phá. 
- Hỗ trợ âm thanh: Nhạc nền và hiệu ứng âm thanh làm tăng trải nghiệm, tạo cảm giác thư giãn hoặc hồi hộp khi cần.
- Yếu tố thời gian: thời gian giới hạn, tạo cảm giác khẩn trương, tăng tính thử thách và kích thích người chơi, áp lực thời gian trong "Hop to Home" làm game hấp dẫn hơn. 
- Hệ thống điểm số: Điểm số dựa trên thời gian hoàn thành có thể khuyến khích người chơi cạnh tranh và chơi lại. 
# Source code game:
- GameAssets.cpp/.h: Tải và quản lý tài nguyên (hình ảnh, âm thanh), giúp các thành phần khác dễ dàng truy cập mà không cần lặp lại mã.
- Level.cpp/.h: Định nghĩa cấu trúc level (vị trí thỏ, nhà, chướng ngại vật), cho phép thêm hoặc chỉnh sửa level mà không ảnh hưởng đến logic chính.
- Menu.cpp/.h: Tách biệt giao diện menu khỏi logic game, giúp dễ dàng cập nhật giao diện mà không can thiệp vào phần chơi.
- Game.cpp/.h: Tập trung logic chính (render, xử lý input, kiểm tra thắng/thua), là "trái tim" của trò chơi.
- Utils.cpp/.h: Cung cấp các hàm hỗ trợ để tránh lặp mã, tăng tính tái sử dụng.
- Constants.h: Lưu các hằng số để dễ dàng điều chỉnh thông số game (như kích thước màn hình, thời gian).
- Globals.h: Quản lý biến toàn cục, cho phép các thành phần khác nhau của game truy cập trạng thái chung.







