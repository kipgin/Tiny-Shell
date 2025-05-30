#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>
int main(){
    __pid_t pid = fork();
    if(pid == 0 ){
        printf("chao , tien trinh con");
    }
    else{
        waitpid(pid,NULL,0);
        printf("chao, tien trinh cha");
    }
}