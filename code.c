#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

//variables from the assignment
#define CUSTOMER_NUMBER 25

#define CUSTOMER_ARRIVAL_TIME_MIN 1

#define CUSTOMER_ARRIVAL_TIME_MAX 3

#define REGISTER_NUMBER 5

#define COFFEE_TIME_MIN 2

#define COFFEE_TIME_MAX 5


void *customer(void *num); //thread function

int randomize(int min, int max); //fucntion for random numbers in a range

int regis[REGISTER_NUMBER]; //array to keep state of registers
int queue = 0; //number for represent first customer in the queue
pthread_mutex_t lock; //mutex to protect regis array while threads works with it

int main(){
	int Number[CUSTOMER_NUMBER];//array to keep the numbers of threads
	int createTime; // variable to save arrival time of customers
	pthread_t cid[CUSTOMER_NUMBER]; //array of threads with the customers number
	//code that create ID's for threads
	for (int i=0; i<CUSTOMER_NUMBER; i++){
	Number[i] = i;
	}
	//code that states all the registers as free
	for (int i=0; i<REGISTER_NUMBER; i++){
	regis[i] = 0;
	}
	//code that runs threads with customer function and displays random arrival time
	for (int i=0; i<CUSTOMER_NUMBER; i++){
	if(i!=0){
			createTime = randomize(CUSTOMER_ARRIVAL_TIME_MIN , CUSTOMER_ARRIVAL_TIME_MAX);//using randomize function to take random number in range
			sleep(createTime); //wait for arrival except first customer
			printf("CUSTOMER %d IS CREATED AFTER %d SECONDS\n" , i+1 , createTime); //display message about arrived customer
		}
		else printf("CUSTOMER 1 IS CREATED\n");//arrival of the first customer
	pthread_create(&cid[i], NULL, customer, (void *)&Number[i]); //run thread with this arguments
	}
	//join threads to make sure they are all finished work before we will go further
	for (int i=0; i<CUSTOMER_NUMBER; i++) {
	pthread_join(cid[i],NULL);
	}
	
	pthread_mutex_destroy(&lock);//destroy mutex to clean ram
	return 0;
}


void *customer(void *number)
{
	//variable i used in for loop , regflag is sentinel controler for while loop , freereg is variable to memorize index of register where customer takes coffee
	int i , regflag = 1 , freereg;
	int num = *(int *)number;// we save number of customer

	//find free register
	while(regflag){
		if(queue == num)
		while(regflag)
		{
			pthread_mutex_lock(&lock); // lock this part of code to prevent interrupting from other threads
			for(i = 0 ; i < REGISTER_NUMBER ; i++)// we will run this loop until we find free register
			{
				if(regis[i] != 1) // condition to search free register
				{
				freereg = i; // save id of freereg
				regis[i] = 1; // mark register as idle
				regflag = 0; //stopwhile loop
				queue++;
				break; //stop for loop
				}
			}
			pthread_mutex_unlock(&lock); // unlock code
			sleep(0.2);// reduce number of iterations by slowing it down to human scale(to seconds per operation)
		}
	}
	

	printf("CUSTOMER %d GOES TO REGISTER %d\n" , num+1 , freereg); // display information about customer and register
	
	int coffeeTime = randomize(COFFEE_TIME_MIN , COFFEE_TIME_MAX);
	sleep(coffeeTime);
	// register finished making coffee
	printf("CUSTOMER %d FINISHED BUYING FROM REGISTER %d AFTER %d SECONDS\n" , num+1 , freereg, coffeeTime); // display info

	pthread_mutex_lock(&lock); // lock code
	regis[freereg] = 0; // mark register as free
	pthread_mutex_unlock(&lock); // unlock code
}

int randomize(int min, int max) // fucntion for randomizing numbers
{
	srand(time(NULL));
	return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}


