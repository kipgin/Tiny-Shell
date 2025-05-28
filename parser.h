// parser.h
#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>

#define MAX_ARGS 100

typedef struct {
    char *cmd;              // lệnh chính
    char *args[MAX_ARGS];   // tham số
    char *input_file;       // < input
    char *output_file;      // > hoặc >>
    bool append;            // có dùng >>
    bool background;        // chạy nền hay không
} command_t;

command_t parse_command(char *line);
void free_command(command_t *cmd); // để tránh memory leak

#endif
