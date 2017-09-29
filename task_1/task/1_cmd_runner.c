/*
  Write a program which allow you to: 
- run another programs via command line.
- get exit codes of terminated programs

## TIPS:
1. Use "2_fork_wait_exit.c" and "4_exec_dir.c" from examples. Combine them.
2. Parse input string according to the type of exec* (see "man exec").
   a) if execvp is used, string splitting into "path" and "args" is all you need.
3. Collect exit codes via waitpid/WEXITSTATUS.
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>


int main(int argc, char * argv[], char * envp[])
{
	printf("Hello. Use 'q' for exit.\n");
	int q = 0;
	while (q == 0) {
		pid_t pid = fork();
		if (pid < 0) {
			printf("fork() error\n");
			return -1;
		}
		if (pid) {
			int status;
			waitpid(pid, &status, 0);
			int a = WEXITSTATUS(status);
			if (a != 255) {
				printf("Ret code: %d\n", a);
			//	break;
			}
			else {
				printf("See you.\n");
				q = 1;
			//	break;
			}
			
		} else {
			char * str = (char *) malloc (255 * sizeof(char));
			char * * args = (char * *) malloc (4096 * sizeof(char *));
			int i;
			char s[255];
			
			gets(str, 255, stdin);
			
			if (strcmp(str, "q") == 0)
				exit(255);
			else {
			
				for (i = 0; strlen(str) > 0; i++) {
					sscanf(str, "%s", s);
					args[i] = (char *) malloc (255 * sizeof(char));
					strcpy(args[i], s);
					str += strlen(s)+1;
				}
						
				execvp(args[0], &args[0]);  
				printf("Error on start\n");
				exit(42);
				
			}
		}
	}
	return 0;
}
