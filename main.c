#include "signal.h"
#include "jobs.h"
#include "builtins.h"

#include <stdio.h>
int main(){
    init_jobs();

    while(1){
        printf("tinyshell> ");
        fflush(stdout);
    }
}
