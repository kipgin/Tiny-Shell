# Tiny-Shell IT3070  
### Nguyễn Ngọc Trung 20230074-ITTN K68  
### Nguyễn Hà Anh Tuấn 20230076-ITTN K68  
### Vũ Đức Tâm 20230064-ITTN K68  
  
Hướng dẫn sử dụng :  
- Bước 1: Gõ vào terminal:  
`gcc -o mysh main.c parser.c executor.c jobs.c signals.c builtins.c variables.c -Wall`  
- Bước 2: Gõ vào terminal:  
`./mysh`  
  
### Một số lệnh cài đặt :  
1. Các thao tác với thư mục :  
- Change directory: `cd /duong/dan`  
- Lùi về thư mục cha: `cd -`  
- Quay lại thư mục trước đó: `cd -`  
- Xem thư mục hiện tại: `pwd`  
- Tạo thư mục tên newfolder: `mkdir newfolder`  
- Xóa thư mục rỗng myfolder: `rmdir myfolder`  
- Xóa thư mục và tất cả thông tin trong đó: `rm -r newfolder`  
- Liệt kê thư mục: `ls`, nếu chi tiết hơn thì sử dụng `ls -l`, còn nếu muốn hiển thị các file ẩn thì sử dụng `ls -a`  
- Hiển thị cây cấu trúc thư mục: `tree`  

  
