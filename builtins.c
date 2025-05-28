#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "variables.h"
#include "builtins.h"

void handle_cd(const char* path) {
    if (chdir(path) != 0) {
        perror("cd failed");
    }
}

void handle_exit() {
    printf("Exiting shell. Goodbye!\n");
    exit(0);
}

void handle_help_builtins() {
    printf("TinyShell Help:\n");
    printf("  cd <dir>      - Change directory\n");
    printf("  exit          - Exit shell\n");
    printf("  help          - Display this help message\n");
    printf("  echo <args>   - Print arguments\n");
    printf("  export NAME=VALUE - Set environment variable\n");
}


// phan nay giong ham  void execute_variable() trong variable.c
void handle_echo(char** hehe) {
    for (int i = 1; hehe[i] != NULL; i++) {
        if (hehe[i][0] == '$') {
            char* val = get_variable(hehe[i] + 1);
            if (val) printf("%s ", val);
        } else {
            printf("%s ", hehe[i]);
        }
    }
    printf("\n");
}

void handle_export(const char* hehe) {
    char name[MAX_VAR_NAME], value[MAX_VAR_VALUE];

    if (sscanf(hehe, "%63[^=]=%255[^=]", name, value) == 2) {
        set_variable(name, value);
    } else {
        fprintf(stderr, "export: failed\n");
    }
}