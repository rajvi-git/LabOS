#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	int pipefd[2];
	int ret;
	char buffer[13];

	// 0 : read end, 1 : write end
	pipe(pipefd);

	if(fork() > 0) {
		printf("I am Parent.\n");
		dprintf(pipefd[1], "Hello World!");
		read(pipefd[0], buffer, sizeof(buffer));
		printf("Message received by parent is : %s\n", buffer);

		wait(NULL);
	} else {
		sleep(1); // wait for parent to finish writing into the pipe
		printf("I am Child.\n");
		read(pipefd[0], buffer, sizeof(buffer));
		printf("Message received by child is : %s\n", buffer);
        while(buffer[i]!='\0'){
            if(buffer[i]<='Z' && buffer[i]>='A'){
                buffer[i]+=32;
            }
            else if(buffer[i]>='a'&&buffer[i]<='z'){
                buffer[i]-=32;
            }
        }
		dprintf(pipefd[1], buffer);

	}

	return 0;
}

