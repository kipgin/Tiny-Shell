#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "parser.h"

//execute command tra ve boi parser
int execute_command(command_t *cmd);

// check xem co phai builtin hay khong
bool is_builtin(const char *cmd);

// execute_builtin
int execute_builtin(command_t *cmd);

#endif