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
int main(int argc, char *argv[]) {
    if (argc >= 3 && strcmp(argv[1], "--internal-run") == 0) {
        // Ghép các đối số còn lại thành chuỗi lệnh
        char command_line[MAX_INPUT] = {0};
        for (int i = 2; i < argc; i++) {
            strcat(command_line, argv[i]);
            if (i < argc - 1) strcat(command_line, " ");
        }

        command_t cmd = parse_command(command_line);
        execute_command(&cmd);
        free_command(&cmd);
        return 0;
    }

    // Shell tương tác như bình thường
    char input[MAX_INPUT];
    
    init_jobs();
    init_signals();
    load_variables();
    printf("Tiny Shell started. Type 'help' for available commands.\n");
    
    while (1) {
        printf("tinyshell> ");
        fflush(stdout);
        
        if (!fgets(input, sizeof(input), stdin)) break;

        if (strlen(input) <= 1) continue;

        command_t cmd = parse_command(input);
        execute_command(&cmd);
        free_command(&cmd);
    }

    printf("\nGoodbye!\n");
    return 0;
}
