#include <iostream>
#include <thread>
#include <semaphore.h>
using namespace std;

sem_t sem;

void thread_func(int id)
{
    cout << "Thread " << id << " is waiting..." << endl;

    sem_wait(&sem);             // Blocking call
    cout << "Thread " << id << " has acquired the semaphore." << endl;

    this_thread::sleep_for(chrono::seconds(2));
    cout << "Thread " << id << " is releasing the semaphore." << endl;
    sem_post(&sem);
}

int main()
{
    const int NUM_THREADS = 5;

    // Initialize the semaphore with the number of threads
    sem_init(&sem, 0, NUM_THREADS);
    thread threads[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; ++i)
    {
        threads[i] = thread(thread_func, i);
    }

    for (int i = 0; i < NUM_THREADS; ++i)
    {
        threads[i].join();
    }

    sem_destroy(&sem);
    return 0;
}