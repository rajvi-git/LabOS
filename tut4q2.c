#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>

int main(int argc, char *argv[]){
    if(argc!=3){
        perror("Enter pattern and filename only");
        return 1;
    }
    pid_t pid = fork();
    if(pid ==0){
        char* args[] ={"search.sh",argv[1],argv[2],NULL};
        execv(args[0],args);
        exit(0);
    }
}