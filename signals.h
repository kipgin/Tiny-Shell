#ifndef SIGNALS_H
#define SIGNALS_H

// Biến toàn cục chứa job_id của foreground process (-1 nếu không có)
extern volatile int fg_job_id;

// set fg process
void set_foreground_job_id(int job_id);

// xoa fg process
void clear_foreground_job_id(void);

// xu li signal SIGINT (Ctrl+C)
void sigint_handler(int sig);

void sigchld_handler(int sig);

// khoi tao signals
void init_signals(void);

#endif