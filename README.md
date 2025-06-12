
# Tiny-Shell IT3070

**Thành viên nhóm:**

- Nguyễn Ngọc Trung - 20230074 - ITTN K68  
- Nguyễn Hà Anh Tuấn - 20230076 - ITTN K68  
- Vũ Đức Tâm - 20230064 - ITTN K68  

---

## Hướng dẫn sử dụng

### Bước 1: Compile chương trình

```bash
gcc -o mysh main.c parser.c executor.c jobs.c signals.c builtins.c variables.c -Wall
```

### Bước 2: Chạy shell

```bash
./mysh
```

---

## Các lệnh hỗ trợ

### 1. **Thao tác với thư mục**

| Lệnh | Mô tả |
|------|------|
| `cd /duong/dan` | Di chuyển đến thư mục chỉ định |
| `cd -` | Quay lại thư mục trước đó |
| `pwd` | Hiển thị thư mục hiện tại |
| `mkdir newfolder` | Tạo thư mục mới |
| `rmdir myfolder` | Xóa thư mục rỗng |
| `rm -r newfolder` | Xóa thư mục và tất cả nội dung bên trong |
| `ls` | Liệt kê thư mục |
| `ls -l` | Liệt kê chi tiết các tệp tin |
| `ls -a` | Hiển thị cả file ẩn |
| `tree` | Hiển thị cây thư mục |

---

### 2. **Lệnh tích hợp (built-in commands)**

| Lệnh | Mô tả |
|------|------|
| `help` | Hiển thị danh sách lệnh hỗ trợ |
| `exit` | Thoát khỏi shell |
| `date` | Hiển thị ngày hiện tại |
| `time` | Hiển thị thời gian hiện tại |
| `ls [option] [dir]` | Liệt kê nội dung thư mục |
| `echo $PATH` | In ra giá trị biến môi trường |
| `export PATH=<value>` | Thêm biến môi trường tạm thời |
| `export PATH=<value> save` | Thêm biến môi trường vĩnh viễn (lưu vào hệ thống) |

---

### 3. **Quản lý tiến trình**

| Lệnh | Mô tả |
|------|------|
| `jobs` | Liệt kê tất cả tiến trình do shell quản lý |
| `kill -SIGINT <job_id>` | Kết thúc tiến trình |
| `kill -SIGSTOP <job_id>` | Tạm dừng tiến trình |
| `stop <job_id>` | Dừng tiến trình |
| `kill -SIGCONT <job_id>` | Tiếp tục tiến trình đang ở background |
| `resume <PID>` | Tiếp tục tiến trình đã dừng |
| `bg <job_id>` | Tiếp tục tiến trình đang dừng ở background |
| `fg <job_id>` | Chuyển tiến trình từ background sang foreground |

---

### 4. **Chạy file `.sh`**

| Lệnh | Mô tả |
|------|------|
| `bash <tên file>` | Chạy file shell script (`*.sh`) |
| `./<tên file>.sh` | Chạy file nếu đã được cấp quyền thực thi (`chmod +x`) |
| `run_sh <tên file>.sh` | Chạy file script mà thư mục cha đã có trong danh sách biến môi trường : `env_var.txt` |

---

> **Lưu ý:** Một số lệnh yêu cầu quyền truy cập hệ thống hoặc cần được chạy trên môi trường hỗ trợ như Unix/Linux.
