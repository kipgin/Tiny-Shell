#include "signals.h"
#include "jobs.h"

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

// global variable lưu job_id của foreground process
volatile int fg_job_id = -1;

void set_foreground_job_id(int job_id) {
    fg_job_id = job_id;
}

void clear_foreground_job_id(void) {
    fg_job_id = -1;
}

void sigint_handler(int sig) {
    if (fg_job_id != -1) {
        printf("\n[SIGINT] Terminating foreground process [%d]...\n", fg_job_id);
        kill_job(fg_job_id);
        clear_foreground_job_id();
    } 
    else {
        printf("\nNo foreground process to terminate.\n");
        printf("tinyshell> ");
        fflush(stdout);
    }
}
void sigchld_handler(int sig) {
    int status;
    pid_t pid;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
            int job_id=find_job_id_by_pid(pid);
            if (job_id != -1) {
                printf("\n[Background] Process [%d] finished.\ntinyshell> ", job_id);
                fflush(stdout);
                remove_job(pid);
                
            
        }
    }
}



void init_signals(void) {
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        perror("signal SIGINT");
    }
    
    // Ctrl + / --> tranh bi thoat
    if (signal(SIGQUIT, SIG_IGN) == SIG_ERR) {
        perror("signal SIGQUIT");
    }
    
    // Ctrl Z cho main shell
    if (signal(SIGTSTP, SIG_IGN) == SIG_ERR) {
        perror("signal SIGTSTP");
    }
    
    signal(SIGCHLD, sigchld_handler);

    
}