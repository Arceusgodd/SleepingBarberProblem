#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_CUSTOMERS 25
void *customer(void *num);
void *barber(void *);
void randwait(int secs);
sem_t waitingRoom;
sem_t barberChair;
sem_t barberPillow;
sem_t seatBelt;
int allDone = 0;

int main(int argc, char *argv[]) {
	pthread_t btid;
	pthread_t tid[MAX_CUSTOMERS];
	long RandSeed;
	int i, numCustomers, numChairs;
	int Number[MAX_CUSTOMERS];
	printf("-------SLEEPING BARBER'S PROBLEM--------");
	printf("\nEnter the number of Customers : ");
	 scanf("%d",&numCustomers) ;
	printf("----------------------------------------");
	printf("\nEnter the number of Chairs : ");
	 scanf("%d",&numChairs);
	if (numCustomers > MAX_CUSTOMERS) {
		printf("\nThe maximum number of Customers is %d.\n", MAX_CUSTOMERS);
		exit(-1);
	}
	for (i=0; i<MAX_CUSTOMERS; i++) {
		Number[i] = i;
	}
	sem_init(&waitingRoom, 0, numChairs);
	sem_init(&barberChair, 0, 1);
	sem_init(&barberPillow, 0, 0);
	sem_init(&seatBelt, 0, 0);
	pthread_create(&btid, NULL, barber, NULL);
	for (i=0; i<numCustomers; i++) {
		pthread_create(&tid[i], NULL, customer, (void *)&Number[i]);
		sleep(1);
	}
	for (i=0; i<numCustomers; i++) {
		pthread_join(tid[i],NULL);
		sleep(1);
	}
	allDone = 1;
	sem_post(&barberPillow); 
	pthread_join(btid,NULL);
}

void *customer(void *number) {
	int num = *(int *)number;
	printf("----------------------------------------");
	printf("\nCustomer %d leaving for barber shop.\n", num+1);
	randwait(2);
	printf("----------------------------------------");
	printf("\nCustomer %d arrived at barber shop.\n", num+1);
	sem_wait(&waitingRoom);
	printf("----------------------------------------");
	printf("\nCustomer %d entering waiting room.\n", num+1);
	sem_wait(&barberChair);
	sem_post(&waitingRoom);
	printf("----------------------------------------");
	printf("\nCustomer %d waking the barber.\n", num+1);
	sem_post(&barberPillow);
	sem_wait(&seatBelt);
	sem_post(&barberChair);
	printf("----------------------------------------");
	printf("\nCustomer %d leaving barber shop.\n", num+1);
}

void *barber(void *junk) {
	while (!allDone) {
		printf("----------------------------------------");
		printf("\nThe barber is sleeping\n");
		sem_wait(&barberPillow);
		if (!allDone) {
			printf("----------------------------------------");
			printf("\nThe barber is cutting hair\n");
			randwait(2);
			printf("----------------------------------------");
			printf("\nThe barber has finished cutting hair.\n");
			sem_post(&seatBelt);
		}
		else {
			printf("----------------------------------------");
			printf("\nThe barber is going home for the day.\n");
			printf("----------------------------------------");
		}
	}
}

void randwait(int secs) {
	int len;
	len = (int) ((1 * secs) + 1);
	sleep(len);
}

