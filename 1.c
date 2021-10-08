#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<sys/wait.h>

int main(){
	int pfid[2], pfid2[2], count = 0;
	char buf[50];
	char msg[] = "Hi There";
	if(pipe(pfid) == -1 || pipe(pfid2) == -1){
		perror("pipe");
		exit(1);
	}
	
	printf("Parent Process %d\n", getpid());
	
	count = strlen(msg)+1;
	write(pfid[1], msg, count);
	if(!fork()){
		char s[50];
		close(pfid2[0]);
		close(pfid[1]);
		read(pfid[0], s, count);
		close(pfid[0]);
		s[count] = '\0';
		int c = 0;
   		char ch;
	    while (s[c] != '\0') {
			ch = s[c];
			if (ch >= 'A' && ch <= 'Z')
				s[c] = s[c] + 32;
			else if (ch >= 'a' && ch <= 'z')
				s[c] = s[c] - 32;  
		  	c++;  
	   }
	   write(pfid2[1], s, count+1);
	   close(pfid2[1]);
	}
	else{
		wait(NULL);
		read(pfid2[0], buf, count+1);
		printf("%s\n", buf);
		close(pfid2[0]);
		close(pfid2[1]);
		close(pfid[1]);
		close(pfid[0]);
	}
	return 0;
}
