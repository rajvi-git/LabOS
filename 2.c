#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define N  10
int main(int argc, char *argv[]){
	/*Parent and child communication dont need a key*/
	pid_t pid;
	int shmid, *arr, it = 0, sz = N, status;
	if(argc != 2){
		perror("Invalid number of arguments\n");
		exit(1);
	}
	shmid = shmget(IPC_PRIVATE, N*sizeof(int),0666 | IPC_CREAT);
	int start  = atoi(argv[1]);
	arr = (int *)malloc(N*sizeof(int));
	arr[it++] = start;
	while(start != 1){
		pid = fork();
		if(pid < 0){
			perror("Fork Failed\n.");
			exit(1);
		}
		if(pid == 0){
			int new, num;
			if(start&1){
				new = 3*start+1;
			}
			else{
				new = start/2;
			}
			char *msg = (char *)shmat(shmid, 0, 0);
			num = (int)((ceil(log10(new))+1)*sizeof(char));
			char *str = (char *)malloc(num*sizeof(char));
			sprintf(str, "%d", new);
			strcpy(msg, str);
			
			shmdt(msg);
			exit(0);
		}
		else{
			if(waitpid(pid, &status, 0) < 0){
				perror("Failed\n");
				kill(pid, SIGTERM);
				exit(1);
			}
			if (!(WIFEXITED(status) && (WEXITSTATUS(status) == 0))){
		        fprintf( stderr,"Child failed. Killing all running children.\n");
		       	kill(pid, SIGTERM);
		        exit(1);
		    }
		    char *msg = (char *)shmat(shmid, (void *)0, 0);
		    start = atoi(msg);
		    if(it == sz){
		    	sz += N;
		    	arr = (int*)realloc(arr, sz*sizeof(int));	
		    }
		    arr[it++] = start;
		}
	}
	for(it = 0; it < sz; it++){
		printf("%d ", arr[it]);
		if(arr[it] == 1)
			break;
	}
	printf("\n");
/*	while(wait(NULL) != -1);*/
	shmctl(shmid, IPC_RMID, NULL);
	return 0;
}
