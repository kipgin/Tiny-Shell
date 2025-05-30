#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

command_t parse_command(char *line) {
    command_t cmd = {0};
    char *tokens[MAX_ARGS];
    int token_count = 0;
    
    
    cmd.cmd = NULL;
    cmd.input_file = NULL;
    cmd.output_file = NULL;
    cmd.append = false;
    cmd.background = false;
    
    
    line[strcspn(line, "\n")] = '\0';
    
    
    int len = strlen(line);
    if (len > 0 && line[len - 1] == '&') {
        cmd.background = true;
        line[len - 1] = '\0';  // xoa '&'

        while (len > 0 && isspace(line[len - 1])) {
            line[--len] = '\0';
        }
    }
    
    
    char *token = strtok(line, " \t");
    while (token != NULL && token_count < MAX_ARGS - 1) {
        tokens[token_count++] = token;
        token = strtok(NULL, " \t");
    }
    tokens[token_count] = NULL;
    
    if (token_count == 0) {
        return cmd;  
    }
    
    
    int arg_index = 0;
    for (int i = 0; i < token_count; i++) {
        if (strcmp(tokens[i], "<") == 0) {
            // Input redirection
            if (i + 1 < token_count) {
                cmd.input_file=strdup(tokens[i + 1]); 
                i++;  
            }
        } else if (strcmp(tokens[i], ">") == 0) {
            // Output redirection
            if (i + 1 < token_count) {
                cmd.output_file = strdup(tokens[i + 1]);
                cmd.append = false;
                i++;  
            }
        } else if (strcmp(tokens[i], ">>") == 0) {
            // Append redirection
            if (i + 1 < token_count) {
                cmd.output_file = strdup(tokens[i + 1]);
                cmd.append = true;
                i++;  
            }
        } else {
            
            if (cmd.cmd == NULL) {
                cmd.cmd = strdup(tokens[i]);
                cmd.args[arg_index++] = cmd.cmd;
            } else {
                cmd.args[arg_index++] = strdup(tokens[i]);
            }
        }
    }
    cmd.args[arg_index] = NULL;
    
    return cmd;
}

void free_command(command_t *cmd) {
    if (cmd->cmd) {
        free(cmd->cmd);
        cmd->cmd = NULL;
    }
    
    
    for (int i = 1; cmd->args[i] != NULL; i++) {
        free(cmd->args[i]);
        cmd->args[i] = NULL;
    }
    
    if (cmd->input_file) {
        free(cmd->input_file);
        cmd->input_file = NULL;
    }
    
    if (cmd->output_file) {
        free(cmd->output_file);
        cmd->output_file = NULL;
    }
}