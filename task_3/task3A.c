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


double f(double x) {
	return x*x;
}

int point() {
	double x = (double) rand()/RAND_MAX; //0 < x < 1
	double y = (double) rand()/RAND_MAX; //0 < y < 1
	if (y - f(x) < 0)
		return 1;
	else
		return 0;
}


void * count (void * number) {
	int * j = number;
	int jj = *j;
	
	int shmid;
	char pathname[] = "easy_to_type";
	key_t key = ftok(pathname, 0);
	shmid = shmget(key, n*sizeof(int), 0);
	
	int * result = (int *)shmat(shmid, NULL, 0);
	
	int i;
	int k = round(N/n);
	for (i = 0; i < k; i++) {
		result[jj] += point();
	}
	shmdt(result);
	return 0;
}


int main(void) {
	
	int shmid;
	char pathname[] = "easy_to_type";
	key_t key = ftok(pathname, 0);
//	printf("%d\n", key);
	shmid = shmget(key, n*sizeof(int), 0666|IPC_CREAT|IPC_EXCL);
//	printf("%d\n", shmid);
	
	int * result = (int *)shmat(shmid, NULL, 0);
	int i;
	for (i = 0; i < n; i++) {
		result[i] = 0;
	}
	shmdt(result);
	
	
	
	srand(time(NULL));
	
	pthread_t* threads;
	int threads_number = n;
	
	threads = (pthread_t*) malloc(threads_number * sizeof(pthread_t));
	int * numbers = (int *) malloc(threads_number * sizeof(pthread_t));
	for (i = 0; i < threads_number; i++)
	{
		numbers[i] = i;
		pthread_create( &(threads[i]), NULL, count, &numbers[i]);
	}
	for (i = 0; i < threads_number ; i++)
	{
		void * res = NULL;
		pthread_join(threads[i], &res);
	}
	free(threads);
	
	
	return 0;
}
