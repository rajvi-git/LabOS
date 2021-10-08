#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include<stdlib.h>

#define SIZE 1024
int A[] = {2,5,6,8,9};
int main(int argc, char* argv[]){
    pid_t child=fork();
    if(child<0){
        perror("fork failed");
        exit(1);
    }
    if(child ==0){
        int retval, shmid;
	    void *memory = NULL;
	    int *message;
        key_t key = ftok("2018B4A70820G_pgm1.c", 0);

        shmid = shmget(key, SIZE, 0666|IPC_CREAT);
        if (shmid < 0) {
            printf("Failed to create shared memory.\n");
            exit (1);
        }
        //printf("Shared memory is at %d\n", shmid);

        // Attach
        memory = shmat(shmid, NULL, 0);
        if(memory == NULL) {
            printf("Attach failed.\n");
            exit(1);
        }

        message = (int *)memory;
        memset(message, '\0', SIZE);
        int squares[5];
        for(int i = 0; i<5;i++){
            squares[i]=A[i]*A[i];
        }
        memcpy(message, squares, sizeof(squares));

        // Detach
        retval = shmdt(memory);
        if(retval < 0) {
            printf("Detach failed.\n");
            exit( 1);
        }

        exit(0);        
    }
    else{
        sleep(1);
        int retval, shmid;
        int *memory;
        int *message;

        // Initialization
        key_t key = ftok("2018B4A70820G_pgm1.c", 0);

        shmid = shmget(key, SIZE, 0666);
        if(shmid < 0) {
            printf("Failed to get shared memory.\n");
            exit( 1);
        }
        printf("Shared memory is at %d\n", shmid);

        // Attach
        memory = (int *)shmat(shmid, NULL, 0);
        if(memory == NULL) {
            printf("Attach failed.\n");
            exit(1);
        }

        message = (char *)memory;
        for(int i =0;i<5;i++){
            printf("%d\t",memory[i]);
        }
        //printf("MESSAGE is \"%s\"\n", message);

        // Detach
        retval = shmdt(memory);
        if(retval < 0) {
            printf("Detach failed.\n");
            exit(1);
        }

        exit(0) ;
    }
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
		printf("Failed to destroy shared memory.\n");
	} else {
		printf("Shared memory destroyed.\n");
	}
}