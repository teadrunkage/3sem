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

#define N 100000000
//#define n 10

unsigned int core;

double f(double x) {
	return x*x;
}

int point(unsigned int * core_r) {
	double x = (double) rand_r(core_r)/RAND_MAX; //0 < x < 1
	double y = (double) rand_r(core_r)/RAND_MAX; //0 < y < 1
	if (y - f(x) < 0)
		return 1;
	else
		return 0;
}

struct numb {
	int i;
	int n;
};


void * count (void * NUMB) {
	struct numb * numb1 = NUMB;
	int j = (*numb1).i;
	int n = (*numb1).n;
	
	unsigned int core_r = core + j;
	
	int shmid;
	char pathname[] = "easy_to_type";
	key_t key = ftok(pathname, 0);
	shmid = shmget(key, n*sizeof(int), 0);
	
	int * result = (int *)shmat(shmid, NULL, 0);
	
	int i;
	int k = round(N/n);
	for (i = 0; i < k; i++) {
		result[j] += point(&core_r);
	}
	shmdt(result);
	return 0;
}


int main(int argc, char * argv[]) {
	int n = atoi(argv[1]);
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
	
	clock_t begin_time, end_time;
	begin_time = time(NULL);
	
	threads = (pthread_t*) malloc(threads_number * sizeof(pthread_t));
	
	
	struct numb * NUMB = (struct numb *) malloc(threads_number * sizeof(struct numb));
	for (i = 0; i < threads_number; i++)
	{
		NUMB[i].i = i;
		NUMB[i].n = n;
		pthread_create( &(threads[i]), NULL, count, &NUMB[i]);
	}
	for (i = 0; i < threads_number ; i++)
	{
		void * res = NULL;
		pthread_join(threads[i], &res);
	}
	free(threads);
	
	end_time = time(NULL);
//	double b = difftime(begin_time,end_time);
//	printf("n = %d; time = %ld\n", n, end_time-begin_time);
	return 0;
}
