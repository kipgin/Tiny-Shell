// parser.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"

command_t parse_command(char *line) {
    command_t cmd;
    memset(&cmd, 0, sizeof(command_t));
    cmd.append = false;
    cmd.background = false;

    char *token;
    int argc = 0;

    token = strtok(line, " \t\n");
    while (token != NULL) {
        if (strcmp(token, "<") == 0) {
            token = strtok(NULL, " \t\n");
            if (token) cmd.input_file = strdup(token);
        } else if (strcmp(token, ">>") == 0) {
            token = strtok(NULL, " \t\n");
            if (token) {
                cmd.output_file = strdup(token);
                cmd.append = true;
            }
        } else if (strcmp(token, ">") == 0) {
            token = strtok(NULL, " \t\n");
            if (token) {
                cmd.output_file = strdup(token);
                cmd.append = false;
            }
        } else if (strcmp(token, "&") == 0) {
            cmd.background = true;
        } else {
            // Lưu argument
            cmd.args[argc++] = strdup(token);
        }
        token = strtok(NULL, " \t\n");
    }

    if (argc > 0) {
        cmd.cmd = strdup(cmd.args[0]);
        cmd.args[argc] = NULL;  // execvp cần NULL ở cuối
    }

    return cmd;
}

void free_command(command_t *cmd) {
    if (cmd->cmd) free(cmd->cmd);
    for (int i = 0; cmd->args[i] != NULL; i++) {
        free(cmd->args[i]);
    }
    if (cmd->input_file) free(cmd->input_file);
    if (cmd->output_file) free(cmd->output_file);
}
