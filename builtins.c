#define _POSIX_C_SOURCE 200809L // do cac hang so cua "time.h" duoc su dung theo chuan POSIX --> chuyen kieu --> compiler moi hieu

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <dirent.h>
#include "builtins.h"
#include "jobs.h"
#include "variables.h"
#define MAX_ENVS 160

static char*temp_envs_var[MAX_ENVS];
static int num_envs_var=0;

void handle_cd(const char*path){
    if(chdir(path) != 0){
        perror("Change Directory failed.");
    }
}

void handle_exit(int argc, char*argv[]){
    int exit_code=0;
    if(argc > 1){
        exit_code=atoi(argv[1]);
    }
    printf("Exiting shell with code %d.\n",exit_code);
    exit(exit_code);
}

void handle_help(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Available built-in commands:\n");
        printf("  cd <dir>                 - Change directory\n");
        printf("  exit [exit_code]         - Exit shell with an optional exit code\n");
        printf("  help [command]           - Display help for a command\n");
        printf("  date [format]            - Display current date/time (format optional)\n");
        printf("  time <command> [args]    - Execute a command and measure elapsed time\n");
        printf("  ls [option] [dir]        - List files in directory (current as default)\n");
        printf("  jobs                     - List processes\n");
        printf("  stop <job_id>            - Stop the process with id=job_id\n");
        printf("  fg <job_id>              - Bring process with id=job_id to foreground\n");
        printf("  bg <job_id>              - Continue process with id=job_id which is currently background\n");
        printf("  kill <job_id>            - Kill process with id=job_id\n");
        printf("  kil -SIGINT <job_id>     - Kill process with id=job_id\n");
        printf("  kill -SIGCONT <job_id>   - Continue stopped process in background with id=job_id\n");
        printf("  bash [filename]          - Execute *.sh file\n");
        printf("  echo $PATH               - Print PATH environment variable\n");
        printf("  export PATH=<value>      - Set PATH environment variable\n");
        printf("  export PATH=<value> save - Set and save PATH environment variable\n");
        return;
    }

    if (strcmp(argv[1], "exit") == 0) {
        printf("exit [exit_code]: Exit the shell with an optional exit code.\n");
    } 
    else if (strcmp(argv[1], "date") == 0) {
        printf("date [format]: Display current date and time. Format uses strftime() directives.\n");
    } 
    else if (strcmp(argv[1], "time") == 0) {
        printf("time <command> [args]: Execute a command and display its elapsed time.\n");
    } 
    else if (strcmp(argv[1], "ls") == 0) {
        printf("ls [option] [dir]: List files in a directory. The option is not processed in this built-in.\n");
    }
    else if (strcmp(argv[1], "echo") == 0) {
        printf("echo $PATH: Print the value of PATH environment variable.\n");
    } 
    else if (strcmp(argv[1], "export") == 0) {
        printf("export PATH=<value>: Set the PATH environment variable to a new value.\n");
    }
    else if(strcmp(argv[1],"cd")==0){
        printf("Change the shell working directory.Change the current directory to DIR. The default DIR is the value of the HOME shell variable.\n");
    } 
    else {
        printf("No help available for command: %s\n", argv[1]);
    }
}

// time + command --> length >= 2

void handle_time(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: time <command> [args ...]\n");
        return;
    }

    struct timespec start, end;

    // khoi tao struct timespec start
    if (clock_gettime(CLOCK_MONOTONIC, &start) == -1) {
        perror("clock_gettime");
        return;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    }

    if (pid == 0) {

        execvp(argv[1], &argv[1]);
        perror("execvp");
        exit(1);

    } 
    else {
        // them vao danh sach tien trinh
        int job_id = add_job(pid, "time command");
        
        // job_id --> foreground process
        bring_to_foreground(job_id);

        //khoi tao struct timespect end
        if (clock_gettime(CLOCK_MONOTONIC, &end) == -1) {
            perror("clock_gettime");
            return;
        }

        //answer : 
        double elapsed = (end.tv_sec - start.tv_sec) +
                         (end.tv_nsec - start.tv_nsec) / 1e9; // nanosecond
        printf("Elapsed time: %.3f seconds\n", elapsed);
    }
    
}

void handle_date(int argc, char* argv[]) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char buffer[100];

    
    if (argc > 1) {
        strftime(buffer, sizeof(buffer), argv[1], t);
    } else {
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", t);
    }
    printf("%s\n", buffer);
}


//cu phap ls

// ls                 # Liệt kê file/thư mục trong thư mục hiện tại
// ls -l              # Hiển thị chi tiết
// ls -a              # Hiển thị cả file ẩn
// ls -lh             # Hiển thị chi tiết, kích thước dễ đọc
// ls /etc            # Liệt kê file/thư mục trong /etc
// ls -l /home/kipgin # Hiển thị chi tiết trong thư mục /home/kipgin
// ls -alR            # Hiển thị tất cả, kể cả file ẩn và đệ quy thư mục con

void handle_ls(int argc, char* argv[]) {
    const char* dir;
    // co 1 tham so
    if(argc < 1){
        fprintf(stderr,"Error");
    }
    if (argc < 2) {
        dir = ".";
    } 
    else {
        // Nếu tham số thứ nhất là option (bắt đầu bằng '-') và có tham số thứ hai, lấy thư mục từ argv[2].

        // ls -
        if (argv[1][0] == '-') {
            if (argc < 3){
                dir = ".";
            }

            // ls -l path
            else{
                dir = argv[2];
            }
        } 
        // ls /etc
        else {
            dir = argv[1];
        }
    }
    
    DIR* d = opendir(dir);
    if (!d) {
        perror("opendir");
        return;
    }
    struct dirent *entry;
    while ((entry = readdir(d)) != NULL) {
        // Bỏ qua file ẩn (bắt đầu bằng '.')
        
        if (entry->d_name[0] != '.')
            printf("%s  ", entry->d_name);
    }
    printf("\n");
    closedir(d);
}

void handle_bash(int argc,char* argv[]){
    if(argc < 2 ){
        printf("Usage: bash [filenname].sh .\n");
        return;
    }
    if(strstr(argv[1],".sh")==NULL){
        printf("Must be a .sh file.\n");
        return;
    }
    if(access(argv[1],F_OK) != 0){
        printf("Cannot access file.\n");
        return;
    }
    pid_t pid =fork();
    if(pid < 0){
        printf("Fork failed!\n");
    }
    if(pid == 0){
        execl("/bin/bash", "bash", argv[1], (char *)NULL);
        exit(1);
    }
    else{
        waitpid(pid,NULL,0);
    }

}

// echo & export da co o phan variable nhung van lam lai :D

void handle_echo_path(int argc, char* argv[]) {
    if (argc < 2 || argv[1] == NULL) {
        printf("%d\n",argc);
        fprintf(stderr, "Usage: echo $VAR or echo STRING\n");
        return;
    }
    if(argv[1][0] != '$'){
        printf("%s\n", argv[1]);
    }
    else if(strcmp(argv[1]+1, "PATH") == 0){
        printf("%s\n", getenv("PATH"));
    }
    else{
        char* name = argv[1] + 1;
        char* variable = get_variable(name);
        for(int i = 0 ; i < num_envs_var ;i++){
            if(strcmp(name,temp_envs_var[i])==0){
                printf("%s\n",temp_envs_var[i]);
                return;
            }
        }
        if(variable == NULL){
            printf("%s\n",name);
        }
        else{
            printf("%s\n",variable);
        }
    }
}

void handle_run_sh(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: run_sh <filename.sh>\n");
        return;
    }
    char* script_path = find_script_in_env_paths(argv[1]);
    if (!script_path) {
        printf("Script '%s' not found in any SCRIPTPATH variables or not executable.\n", argv[1]);
        return;
    }
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    }
    if (pid == 0) {
        execl("/bin/bash", "bash", script_path, (char*)NULL);
        perror("execl");
        exit(1);
    } else {
        waitpid(pid, NULL, 0);
    }
}


void handle_export(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: export VARIABLE=<value>\n");
        return;
    }
    
    char *arg = argv[1];
    char *equalSign = strchr(arg, '=');
    if (!equalSign) {
        fprintf(stderr, "export: invalid format, expected PATH=<value>\n");
        return;
    }
    *equalSign = '\0';
    char* var = arg;
    char* value = equalSign + 1;
    if(value==NULL){
        printf("No value to assign.\n");
    }
    int flag=0;
    if(argc == 3){
        if(strcmp(argv[2],"save")==0){
            set_variable(var,value);
            return;
        }
        else{
            printf("Try export VAR=VAL or export VAR=VAL save.\n");
        }
    }
    for(int i = 0 ; i < num_envs_var ;i++){
        if(strcmp(var,temp_envs_var[i])==0){
            temp_envs_var[i] = (char*)malloc(sizeof(char)*strlen(value));
            if(temp_envs_var[i]==NULL){
                printf("Failed to set variable :(\n");
                return;
            }
            else{
                strcpy(temp_envs_var[i],value);
            }
            flag=1;
        }
    }
    if(flag){
        return;
    }
    temp_envs_var[num_envs_var] = (char*)malloc(sizeof(char)*strlen(value));
    if(temp_envs_var[num_envs_var]==NULL){
        printf("Cannot export variable :(\n");
    }
    strcpy(temp_envs_var[num_envs_var],value);
    num_envs_var++;

}