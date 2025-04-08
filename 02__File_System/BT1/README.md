**BT1**. Viết một chương trình mở một file bất kì và sử dụng cờ `O_APPEND`, sau đó thực hiện *seek* về đầu file rồi ghi một vài dữ liệu vào file đó. Dữ liệu sẽ xuất hiện ở vị trí nào của file và tại sao lại như vậy?

**Trả lời:**

Dữ liệu sẽ được ghi ở **cuối file**, **không phải ở đầu file**, mặc dù đã gọi `lseek()` để di chuyển con trỏ về đầu.

Lý do là vì file được mở với cờ `O_APPEND`. Khi sử dụng `O_APPEND`, mỗi lần ghi (`write()`) sẽ **tự động ghi vào cuối file**, bất kể con trỏ file đang ở đâu. Vì vậy, lệnh `lseek()` không ảnh hưởng đến vị trí ghi trong trường hợp này.
