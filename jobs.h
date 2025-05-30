#ifndef JOBS_H
#define JOBS_H
#include <unistd.h>
#define MAX_JOBS 128

typedef struct {
    pid_t pid;
    char command[256];
    int active;
} Job;

//khoi tao danh sach cac tien trinh
void init_jobs();

//them tien trinh vao danh sach tren ( theo chien luoc First-Fit)
int add_job(pid_t pid, const char* command);

//xoa tien trinh khoi danh sach
void remove_job(pid_t pid);

// in ra tat ca tien trinh
void list_jobs();

//dua 1 tien trinh ra foreground
// job_id : chi so trong danh sach != pid
void bring_to_foreground(int job_id);

//tiep tuc tien trinh bi stop
void resume_job(int job_id);

//stop tien trinh
void stop_job(int job_id);

//dung tien trinh
void kill_job(int job_id);

// xau dau vao -> chia ra cac truong hop de chay tien trinh
// -> if-else -> co the chay o foreground hoac background
void execute_job(const char**tokens,int num_tokens);

#endif