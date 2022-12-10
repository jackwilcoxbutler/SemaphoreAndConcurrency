#include <semaphore.h>
#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>

using namespace std;

// total number of customers
#define NUM_CUSTOMERS 50

// maximum length of the line waiting for service
// when 1 person is being served, and 2 more are 
// waiting, new customers will leave
#define MAX_OCCUPANCY 10

// variables to track the number of customers in line 
// and total number who have left or been served
int waiting = 0;
int served  = 0;

// you'll also need some semaphores (at least 4)
// declare them globally and then initialize them in main()

// 1. two to safely update the shared variables waiting and served
// 2. one to signal customers
// 3. one to signal the barista

//waiting
sem_t sem_waiting;
//served
sem_t sem_served;
//signal customers
sem_t sem_sig_c;
//signal barista
sem_t sem_sig_b;


// simulate time passing
void random_wait(int time)
{
  int length = random() % time;
  sleep(length);
}


void *barista_thread(void *args)
{
  // only serve the number of customers that we expect 
  // in this run, then close the cafe
  sem_wait(&sem_served);
  while(served < NUM_CUSTOMERS) {
    sem_post(&sem_served);
     
    // Wait for a customer to arrive
    sem_wait(&sem_sig_b);
    // if(served < NUM_CUSTOMERS)
    // take the customer from the line
    

    // make a coffee
    random_wait(7);
    sem_post(&sem_sig_c);
    sem_wait(&sem_served);
    served += 1;
    cout << served << " Done." << endl;

  }
  
  pthread_exit(0);
}


void *customer_thread(void *arg)
{
  int i = *((int *)arg);

  /* wait a random time before going to the cafe */
  random_wait(10);
  cout << "[Customer " << i << "] **** arrived. ****" << endl;

  // Each customer should try to get one (1) coffee
  sem_wait(&sem_waiting);
  if (waiting >= MAX_OCCUPANCY) 
  {
    sem_post(&sem_waiting);
    cout << "[Customer " << i << "] **** leaving... Line too long ****" << endl;
    sem_wait(&sem_served);
    served += 1;
    cout << served << " Done." << endl;
    sem_post(&sem_served);

    pthread_exit(0);
  } else {
    // join the line to get a coffee
    waiting++;
    sem_post(&sem_waiting);

    // signal the barista in case I'm the only customer here
    sem_post(&sem_sig_b);

    // wait for my coffee
    sem_wait(&sem_sig_c);

    cout << "[Customer " << i << "] **** Served & leaving. ****" << endl;
    sem_wait(&sem_waiting);
    waiting--;
    sem_post(&sem_waiting);
  }
  pthread_exit(0);
}


int main()
{
  sem_init(&sem_waiting, 0, 1);
  sem_init(&sem_served, 0, 1);
  sem_init(&sem_sig_c, 0, 0);
  sem_init(&sem_sig_b, 0, 0);
  
  // seed the random number generator
  srandom(2688);

  pthread_t barista;
  pthread_create(&barista, NULL, barista_thread, NULL);

  cout << "Creating " << NUM_CUSTOMERS << " customer threads." << endl;
  cout << "There are/is " << (MAX_OCCUPANCY-1) << "spot(s) in the line." << endl;
  
  pthread_t customers[NUM_CUSTOMERS];
  int customer_tids[NUM_CUSTOMERS] = {};

  for(int i = 0; i < NUM_CUSTOMERS; i++)
  {
    customer_tids[i] = i;
    pthread_create(&customers[i], NULL, customer_thread, (void *)&customer_tids[i]);
  }

  for(int i = 0; i < NUM_CUSTOMERS; i++)
  {
    pthread_join(customers[i], NULL);
  }
  pthread_join(barista, NULL);
  cout << "The cafe is now closed!" << endl;
  
  sem_destroy(&sem_waiting);
  sem_destroy(&sem_served);
  sem_destroy(&sem_sig_c);
  sem_destroy(&sem_sig_b);

  exit(0);
}



/*
barista holds c signal
customer walks in
c signals barista
customer waits on c signal
barista makes coffee
spends time

barista posts c signal
customer 




*/