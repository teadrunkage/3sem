/*
Write a duplex pipe implementation.
typedef struct {
  int txd[2]; 
  int rxd[2]; 
} dpipe_t;
1) This pipe should connect some process with his child, for continuous communication.
2) Be careful with opened descriptors.
3) When one process is terminated, the other should also exit.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


typedef struct {
  int txd[2]; 
  int rxd[2]; 
} dpipe_t;


int main()	{
	
	dpipe_t mypipe;
	size_t size;
	int pid;
	char str[4096];
	char restr[4096];
	
	
	if (pipe(mypipe.txd) < 0) {
		printf("Can't create pipe\n");
		exit(-1);
	}
	
	if (pipe(mypipe.rxd) < 0) {
		printf("Can't create pipe\n");
		exit(-1);
	}
	
	pid = fork();
	if (pid < 0) {
		printf("Can't fork\n");
		exit(-1);
	}
	else if (pid > 0) {		//parent wrights to child
		
		close(mypipe.txd[0]);
		printf("What will the parent say to the child?\n");
		gets(str);
		size = write(mypipe.txd[1], str, sizeof(str));
		if (size != sizeof(str)) {
			printf("Can't wright the string\n");
			exit(-1);
		}
		close(mypipe.txd[1]);
		
		
		
		
		int status;
		waitpid(pid, &status, 0);
		
		
		
		close(mypipe.rxd[1]);
		size = read(mypipe.rxd[0], restr, sizeof(str));
		if(size < 0) {
			printf("Can't read the string\n");
			exit(-1);
		}
		printf("\"%s,\" - reads the parent.\n", restr);
		close(mypipe.rxd[0]);
		
	}
		else {					//child reads from the parent
			
			close(mypipe.txd[1]);
			size = read(mypipe.txd[0], restr, sizeof(str));
			if(size < 0) {
				printf("Can't read the string\n");
				exit(-1);
			}
			printf("\"%s,\" - reads the child.\n", restr);
			close(mypipe.txd[0]);
			
			
			close(mypipe.rxd[0]);
			printf("What will the child say to the parent?\n");
			gets(str);
			size = write(mypipe.rxd[1], str, sizeof(str));
			if (size != sizeof(str)) {
				printf("Can't wright the string\n");
				exit(-1);
			}
			close(mypipe.rxd[1]);
		}
		
		
	
	return 0;
}
