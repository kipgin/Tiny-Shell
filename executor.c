#include "executor.h"
#include "builtins.h"
#include "jobs.h"
#include "signals.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

bool is_builtin(const char *cmd) {
    if (!cmd) return false;
    
    return (strcmp(cmd, "cd") == 0 ||
            strcmp(cmd, "exit") == 0 ||
            strcmp(cmd, "help") == 0 ||
            strcmp(cmd, "date") == 0 ||
            strcmp(cmd, "time") == 0 ||
            strcmp(cmd, "ls") == 0 ||
            strcmp(cmd, "echo") == 0 ||
            strcmp(cmd, "export") == 0 ||
            strcmp(cmd, "jobs") == 0 ||
            strcmp(cmd, "fg") == 0 ||
            strcmp(cmd, "bg") == 0 ||
            strcmp(cmd, "stop") == 0 ||
            strcmp(cmd, "kill") == 0);
}

int execute_builtin(command_t *cmd) {
    if (!cmd || !cmd->cmd) return -1;
    
    // count args 's tokens
    int argc = 0;
    while (cmd->args[argc] != NULL) argc++;
    
    // Thực thi lệnh built-in tương ứng
    if (strcmp(cmd->cmd, "cd") == 0) {
        const char *path = (argc > 1) ? cmd->args[1] : getenv("HOME");
        handle_cd(path ? path : ".");
    } else if (strcmp(cmd->cmd, "exit") == 0) {
        handle_exit(argc, cmd->args);
    } else if (strcmp(cmd->cmd, "help") == 0) {
        handle_help(argc, cmd->args);
    } else if (strcmp(cmd->cmd, "date") == 0) {
        handle_date(argc, cmd->args);
    } else if (strcmp(cmd->cmd, "time") == 0) {
        handle_time(argc, cmd->args);
    } else if (strcmp(cmd->cmd, "ls") == 0) {
        handle_ls(argc, cmd->args);
    } else if (strcmp(cmd->cmd, "echo") == 0) {
        if (argc > 1 && strcmp(cmd->args[1], "$PATH") == 0) {
            handle_echo_path();
        } else {
            printf("echo: only $PATH supported\n");
        }
    } else if (strcmp(cmd->cmd, "export") == 0) {
        handle_export(argc, cmd->args);
    } else {
        // Job control commands
        const char *tokens[MAX_ARGS];
        for (int i = 0; i < argc && i < MAX_ARGS; i++) {
            tokens[i] = cmd->args[i];
        }
        execute_job(tokens, argc);
    }
    
    return 0;
}


// trong truong hop co redirect
int setup_redirection(command_t *cmd) {
    // Input redirection
    if (cmd->input_file) {
        int fd = open(cmd->input_file, O_RDONLY);
        if (fd < 0) {
            perror("open input file");
            return -1;
        }
        if (dup2(fd, STDIN_FILENO) < 0) {
            perror("dup2 input");
            close(fd);
            return -1;
        }
        close(fd);
    }
    
    // Output redirection
    if (cmd->output_file) {
        int flags = O_WRONLY | O_CREAT;
        flags |= cmd->append ? O_APPEND : O_TRUNC;
        
        int fd = open(cmd->output_file, flags, 0644);
        if (fd < 0) {
            perror("open output file");
            return -1;
        }
        if (dup2(fd, STDOUT_FILENO) < 0) {
            perror("dup2 output");
            close(fd);
            return -1;
        }
        close(fd);
    }
    
    return 0;
}

int execute_command(command_t *cmd) {
    //empty command
    if (!cmd || !cmd->cmd) {
        return 0;  
    }
    
    // check if builtin command :<
    if (is_builtin(cmd->cmd)) {
        return execute_builtin(cmd);
    }
    
    // thuc thi may cai lenh chua cai dat --> cho vao execvp(args[0],args)
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return -1;
    }
    
    if (pid == 0) {
        // tien trinh con
        
        // if redirection
        if (setup_redirection(cmd) < 0) {
            exit(1);
        }
        
        // execute command
        execvp(cmd->cmd, cmd->args);
        perror("execvp");
        exit(1);
    } else {
        // tien trinh cha
        
        // them process vao danh sach job da cai dat
        char command_str[512] = {0};
        strncpy(command_str, cmd->cmd, sizeof(command_str) - 1);
        for (int i = 1; cmd->args[i] != NULL; i++) {
            strncat(command_str, " ", sizeof(command_str) - strlen(command_str) - 1);
            strncat(command_str, cmd->args[i], sizeof(command_str) - strlen(command_str) - 1);
        }
        

        int job_id = add_job(pid, command_str);
        if (job_id < 0) {
            fprintf(stderr, "Failed to add job\n");
            return -1;
        }
        
        if (cmd->background) {
            // Back process
            printf("[%d] %d %s &\n", job_id, pid, command_str);
        } else {
            // Foreground process
            bring_to_foreground(job_id);
        }
    }
    
    return 0;
}