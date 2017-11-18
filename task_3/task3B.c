#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <sys/wait.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

#define N 1000000
#define n 1024


int main(void) {
	
	int shmid;
	char pathname[] = "easy_to_type";
	key_t key = ftok(pathname, 0);
//	printf("%d\n", key);
	shmid = shmget(key, n*sizeof(int), 0);
//	printf("%d\n", shmid);
	
	int * result = (int *)shmat(shmid, NULL, 0);
	
	int i;
	double b = 0;
	for (i = 0; i < n; i++) {
		b += result[i];
	}
	printf("%f\n", (double) b/N);
	
	shmdt(result);
	shmctl(shmid, IPC_RMID, NULL);
	return 0;
}
