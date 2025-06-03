#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "variables.h"

#define ENV_FILE "env_var.txt"

static ShellVar variables[MAX_VARS];
static int var_count = 0;

void load_variables() {
    FILE *file = fopen(ENV_FILE, "r");
    if (!file) return;

    var_count = 0;
    while (fscanf(file, "%63[^=]=%255[^\n]\n", variables[var_count].name, variables[var_count].value) == 2) {
        var_count++;
        if (var_count >= MAX_VARS) break;
    }

    fclose(file);
}

void save_variables() {
    FILE *file = fopen(ENV_FILE, "w");
    if (!file) return;

    for (int i = 0; i < var_count; i++) {
        fprintf(file, "%s=%s\n", variables[i].name, variables[i].value);
    }

    fclose(file);
}

char* get_variable(const char* name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            return variables[i].value;
        }
    }
    return NULL;
}

void set_variable(const char* name, const char* value) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            strncpy(variables[i].value, value, MAX_VAR_VALUE);
            save_variables();
            return;
        }
    }
    
    if (var_count < MAX_VARS) {
        strncpy(variables[var_count].name, name, MAX_VAR_NAME);
        strncpy(variables[var_count].value, value, MAX_VAR_VALUE);
        var_count++;
        save_variables();
    }
}

void print_variables() {
    for (int i = 0; i < var_count; i++) {
        printf("%s=%s\n", variables[i].name, variables[i].value);
    }
}

//ham nay execute 
void execute_variables(char* line){
    char*first_token = strtok(line,"\n\t ");
    if(strcmp(first_token,"export") ==0){
        char*sec_token= strtok(NULL,"\t\n ");
        char*variable = malloc(sizeof(char)*30);
        strcpy(variable,sec_token);
        char*findcharacter = strchr(variable,'=');
        char*value=findcharacter+1;
        variable[findcharacter-variable]='\0';
        set_variable(variable,value);
    }
    else if(strcmp(first_token,"echo")==0){
        char*sec_token=strtok(NULL,"\t\n ");
        if(sec_token[0] != '$'){
            printf("Ban can nhap dau $ truoc bien");
            return;
        }
        sec_token+=1;
        if(get_variable(sec_token) == NULL){
            printf("%s\n",sec_token);
            return;
        }
        printf("%s\n",get_variable(sec_token));
    }
    
}
