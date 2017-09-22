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

int main(int argc, char * argv[], char * envp[])
{
	const pid_t pid = fork();
	if (pid < 0) {
		printf("fork() error\n");
		return -1;
	}
	if (pid) {
		int status;
		waitpid(pid, &status, 0);
		printf("Ret code: %d\n", WEXITSTATUS(status));
	} else {
		execvp(argv[1], &argv[1]);  //выйдет через return
		printf("Error on start\n"); //если не запустилось, то 42
		exit(42);
	}
	return 0;
}
