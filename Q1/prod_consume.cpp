#include <iostream>
#include <thread>
#include <semaphore.h>
using namespace std;

const int BUFFER_SIZE = 5;
int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

sem_t emptySpaces;                      // Counts number of empty slots in the buffer
sem_t full;                             // Counts number of filled slots in the buffer
sem_t mutex;                            // Binary semaphore to protect access to the buffer

void producer()
{
    int item = 0;
    while (true)
    {
        // Produce an item
        item++;
        sem_wait(&emptySpaces);         // Wait for an empty slot in the buffer
        sem_wait(&mutex);               // Acquire mutex lock to protect the buffer

        buffer[in] = item;              // Add the item to the buffer
        in = (in + 1) % BUFFER_SIZE;

        sem_post(&mutex);               // Release mutex lock
        sem_post(&full);                // Signal that there is one more filled slot in the buffer
    }
}

void consumer()
{
    int item;
    while (true)
    {
        sem_wait(&full);                // Wait for an filled slot in the buffer
        sem_wait(&mutex);               // Acquire mutex lock to protect the buffer

        item = buffer[out];             // Remove an item the buffer
        out = (out + 1) % BUFFER_SIZE;

        sem_post(&mutex);               // Release mutex lock
        sem_post(&emptySpaces);         // Signal that there is one more empty slot in the buffer

        // Consume the item
        cout << "Consumed item: " << item << "." << endl;
    }
}

int main()
{
    // Initialize semaphores
    sem_init(&emptySpaces, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    // Create producer and consumer threads
    thread prod_thread(producer);
    thread cons_thread(consumer);

    // Wait for threads to finish (should never happen)
    prod_thread.join();
    cons_thread.join();

    // Destroy semaphores
    sem_destroy(&emptySpaces);
    sem_destroy(&full);
    sem_destroy(&mutex);

    return 0;
}