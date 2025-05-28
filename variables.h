#ifndef VARIABLES_H
#define VARIABLES_H

#define MAX_VARS 100
#define MAX_VAR_NAME 64
#define MAX_VAR_VALUE 256

typedef struct {
    char name[MAX_VAR_NAME];
    char value[MAX_VAR_VALUE];
} ShellVar;

void load_variables();
void save_variables();
char* get_variable(const char* name);
void set_variable(const char* name, const char* value);
void print_variables();

#endif