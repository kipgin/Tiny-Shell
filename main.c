#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "executor.h"
#include "jobs.h"
#include "signals.h"
#include "builtins.h"
#include "redirect.h"
#include "variables.h"

#define MAX_INPUT 1024

int main() {
    char input[MAX_INPUT];
    
    // Khởi tạo
    init_jobs();
    init_signals();
    load_variables();
    printf("Tiny Shell started. Type 'help' for available commands.\n");
    
    while (1) {
        printf("tinyshell> ");
        fflush(stdout);
        
        
        if (!fgets(input, sizeof(input), stdin)) {
            break;  // EOF
        }
        
        
        if (strlen(input) <= 1) {
            continue;
        }
        
        
        command_t cmd = parse_command(input);
        
        
        execute_command(&cmd);
        
        
        free_command(&cmd);
    }
    
    printf("\nGoodbye!\n");
    return 0;
}