#include <semaphore.h>
#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>

using namespace std;

// total number of customers
#define NUM_CUSTOMERS 10

// maximum length of the line waiting for service
// when 1 person is being served, and 2 more are 
// waiting, new customers will leave
#define MAX_OCCUPANCY 2

// variables to track the number of customers in line 
// and total number who have left or been served
int waiting = 0;
int served  = 0;

// you'll also need some semaphores (at least 4)
// declare them globally and then initialize them in main()

// 1. two to safely update the shared variables waiting and served
// 2. one to signal customers
// 3. one to signal the barista


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
  while(served < NUM_CUSTOMERS) {
    // Wait for a customer to arrive
    // take the customer from the line

    // make a coffee
    random_wait(7);

    served += 1;
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
  if (waiting >= MAX_OCCUPANCY) 
  {
    cout << "[Customer " << i << "] **** leaving. ****" << endl;
    served += 1;
    
    pthread_exit(0);
  } else {
    // join the line to get a coffee
    // signal the barista in case I'm the only customer here
    // wait for my coffee
  }
  pthread_exit(0);
}


int main()
{
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
  
  exit(0);
}