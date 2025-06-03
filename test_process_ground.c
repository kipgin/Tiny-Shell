#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_CMD_LEN 1024
#define MAX_ARGS 64

int main() {
    char input[MAX_CMD_LEN];
    char *args[MAX_ARGS];

    while (1) {
        printf("tinyshell> ");
        fflush(stdout);

        // Đọc lệnh
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;  // EOF (Ctrl+D)
        }

        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0) continue;

        if (strcmp(input, "exit") == 0) {
            break;
        }

        // kiem tra co dau '&' hay khong
        int background = 0;
        if (input[strlen(input) - 1] == '&') {
            background = 1;
            input[strlen(input) - 1] = '\0';
        }

        // tach input thanh tokens --> nhet vao execvp()
        int argc = 0;
        char *token = strtok(input, " ");
        while (token != NULL && argc < MAX_ARGS - 1) {
            args[argc++] = token;
            token = strtok(NULL, " ");
        }
        args[argc] = NULL;

        // Tạo tiến trình
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork failed");
            continue;
        }

        if (pid == 0) {
            // child process
            execvp(args[0], args);
            perror("exec failed");
            exit(1);
        } else {
            // parent
            if (background) {
                printf("[Running background] PID: %d\n", pid);
            } else {
                printf("[Running foreground] PID: %d\n", pid);
                waitpid(pid, NULL, 0);
                printf("[Done] %s\n", args[0]);
            }
        }
    }

    printf("tinyshell exit.\n");
    return 0;
}
