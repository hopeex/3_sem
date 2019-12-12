#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

#define NUMBER_OF_POINTS 100000000
#define NUMBER_OF_THREADS 1

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
double sum = 0;1

double func(double x){
	return cos(x);
}

double integral1(long long int points) {//What part of points hit the area
	long long int s = 0;	
	double x;
	double y;
	struct drand48_data randBuffer;
	srand48_r(time(NULL), &randBuffer);//initialization of the struct
	for (int i = 0; i < points; i++) {
		drand48_r(&randBuffer, &x);//generate x in interval from 0 to 1
		drand48_r(&randBuffer, &y); //generate y in interval from 0 to 1
		if(y < func(x))
			s++;
	}
	return s;
}

void *cnt_routine() {
	double sub_sum = integral1(NUMBER_OF_POINTS/NUMBER_OF_THREADS);
	pthread_mutex_lock(&mutex);
	sum += sub_sum;
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}

int main() {
	clock_t begin = clock();
	long long int i;
	pthread_t id[NUMBER_OF_THREADS];
	for (i = 0; i < NUMBER_OF_THREADS; i++){
		pthread_create(&id[i], NULL, cnt_routine, NULL);
	}
	for (i = 0; i < NUMBER_OF_THREADS; i++){
		pthread_join(id[i], NULL);
	}
	clock_t end = clock();
	double time = (double)(end - begin)/CLOCKS_PER_SEC;
	printf("Result of counting: %f\n  time: %f\n ", sum/NUMBER_OF_POINTS, time);
	return 0;
}


