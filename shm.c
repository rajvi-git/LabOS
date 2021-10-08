#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>


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
	    char *message;
        key_t key = ftok("shm.c", 0);

        shmid = shmget(key, SIZE, 0666|IPC_CREAT);
        if (shmid < 0) {
            printf("Failed to create shared memory.\n");
            exit (1);
        }
        printf("Shared memory is at %d\n", shmid);

        // Attach
        memory = shmat(shmid, NULL, 0);
        if(memory == NULL) {
            printf("Attach failed.\n");
            exit(1);
        }

        message = (char *)memory;
        memset(message, '\0', SIZE);
        int squares[5];
        for(int i = 0; i<5;i++){
            squares[i]=A[i]*A[i];
        }
        memcpy(message, (char *squares), strlen(char*squares));

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
        void *memory = NULL;
        char *message;

        // Initialization
        key_t key = ftok("write.c", 0);

        shmid = shmget(key, SIZE, 0666);
        if(shmid < 0) {
            printf("Failed to get shared memory.\n");
            exit( 1);
        }
        printf("Shared memory is at %d\n", shmid);

        // Attach
        memory = shmat(shmid, NULL, 0);
        if(memory == NULL) {
            printf("Attach failed.\n");
            exit(1);
        }

        message = (char *)memory;
        for(int i =0;i<5;i++){
            printf("%d",atoi(message[i]));
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
}