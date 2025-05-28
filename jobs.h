#ifndef JOBS_H
#define JOBS_H
#include <unistd.h>
#define MAX_JOBS 128

typedef struct {
    pid_t pid;
    char command[256];
    int active;
} Job;

void init_jobs();
int add_job(pid_t pid, const char* command);
void remove_job(pid_t pid);
void list_jobs();
void bring_to_foreground(int job_id);
void resume_job(int job_id);
void kill_job(int job_id);

#endif