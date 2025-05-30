#include "jobs.h"
#include "signal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

static Job jobs[MAX_JOBS];

void init_jobs(){
    for(int i = 0 ; i < MAX_JOBS ;i++){
        jobs[i].active=0;
        jobs[i].command[0]='\0';
        jobs[i].pid =0;
    }
}

int add_job(pid_t pid, const char * command){
    //khong can thiet lam vi da xu li o ham parse.c hoac main.c

    for(int i = 0 ; i < MAX_JOBS ;i++){
        if(jobs[i].active==0){
            jobs[i].active=1;
            jobs[i].pid=pid;
            strncpy(jobs[i].command,command,sizeof(jobs[i].command));
            //khong can thiet lam vi da xu li o ham parse.c hoac main.c
            if(jobs[i].command[sizeof(jobs[i].command)-1] == '\n'){
                jobs[i].command[sizeof(jobs[i].command)-1] ='\0';
            }

            return i;
        }
    }
    fprintf(stderr,"Job list is full!\n");
    return -1;
}

void remove_job(pid_t pid){
    for(int i = 0; i < MAX_JOBS;i++){
        if(jobs[i].pid == pid){
            jobs[i].active=0;
            jobs[i].command[0]='\0';
            jobs[i].pid=0;
            return;
        }
    }
}
void list_jobs() {
    printf("JobID\tPID\tActive\tCommand\n");
    for (int i = 0; i < MAX_JOBS; i++) {
        if (jobs[i].pid != 0) {
            printf("[%d]\t%d\t%s\t%s\n", i, jobs[i].pid,
                (jobs[i].active ? "Yes" : "No"), jobs[i].command);
        }
    }
}

void bring_to_foreground(int job_id){
    if(job_id < 0 || job_id >= MAX_JOBS || jobs[job_id].pid==0){
        fprintf(stderr, "Invalid job id.\n");
        return;
    }

    pid_t pid = jobs[job_id].pid;
     
    if (!jobs[job_id].active) {

        //dung thi tra ve = 0
        if (kill(pid, SIGCONT) < 0) {
            perror("SIGCONT failed");
            return;
        }
        jobs[job_id].active = 1;
    }

    // thiet lap process nay la fg process
    set_foreground_job_id(job_id);
    int status; // luu lai status cua child process
    
    // wait process con xong thi moi chay tiep -> foreground process
    if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid failed");
    }

    clear_foreground_job_id();
    remove_job(pid);
    printf("Process [%d] finished.\n", pid);
}

void resume_job(int job_id){
    if(job_id < 0 || job_id >= MAX_JOBS || jobs[job_id].pid==0){
        fprintf(stderr, "Invalid job id.\n");
        return;
    }

    pid_t pid = jobs[job_id].pid;
    if(kill(pid,SIGCONT) < 0){
        perror("SIGCONT failed, cannot resume process!");
        return;
    }

    jobs[job_id].active=1;
    printf("Process [%d] resumed.\n", pid);
}

void stop_job(int job_id){
    if (job_id < 0 || job_id >= MAX_JOBS || jobs[job_id].pid == 0) {
        fprintf(stderr, "Invalid job id.\n");
        return;
    }
    pid_t pid = jobs[job_id].pid;
    if(kill(pid,SIGSTOP) < 0){
        perror("SIGSTOP failed, cannot stop process.");
        return;
    }
    jobs[job_id].active=1;
    printf("Process [%d] stopped",pid);
}

void kill_job(int job_id){
    if (job_id < 0 || job_id >= MAX_JOBS || jobs[job_id].pid == 0) {
        fprintf(stderr, "Invalid job id.\n");
        return;
    }
    pid_t pid =jobs[job_id].pid;
    if(kill(pid,SIGKILL) < 0 ){
        perror("SIGKILL failed, cannot kill process.");
        return;
    }
    printf("Process [%d] killed.",pid);
    remove_job(pid);
}

//xet ra lam cac truong hop

void execute_job(const char** tokens, int num_tokens) {
    if (num_tokens == 0) return;

    if (strcmp(tokens[0], "jobs") == 0) {
        list_jobs();
    } 
    else if (strcmp(tokens[0], "fg") == 0) {
        if (num_tokens < 2) {
            fprintf(stderr, "Usage: fg <job_id>\n");
            return;
        }
        int job_id = atoi(tokens[1]);
        bring_to_foreground(job_id);
    } 
    else if (strcmp(tokens[0], "bg") == 0) {
        if (num_tokens < 2) {
            fprintf(stderr, "Usage: bg <job_id>\n");
            return;
        }
        int job_id = atoi(tokens[1]);
        resume_job(job_id);
    }
    else if (strcmp(tokens[0], "stop") == 0) {
        if (num_tokens < 2) {
            fprintf(stderr, "Usage: stop <job_id>\n");
            return;
        }
        int job_id = atoi(tokens[1]);
        stop_job(job_id);
    }
    else if (strcmp(tokens[0], "kill") == 0) {
        if (num_tokens < 2) {
            fprintf(stderr, "Usage: kill <job_id>\n");
            return;
        }
        if(strcmp(tokens[1],"-SIGCONT")==0 || atoi(tokens[1]+1)==18){
            int job_id = atoi(tokens[2]);
            resume_job(job_id);
        }
        else if(strmcp(tokens[1],"-SIGKILL")==0 || atoi(tokens[1]+1)==9){
            int job_id=atoi(tokens[2]);
            kill_job(job_id);
        }
        else if( atoi(tokens[1]) >= 0  && atoi(tokens[1]) <= MAX_JOBS){
            kill_job(atoi(tokens[1]));
        }
        else{
            fprintf(stderr,"Killed Process failed.");
        }
    }
    else {
        fprintf(stderr, "Unknown job command: %s\n", tokens[0]);
    }
}