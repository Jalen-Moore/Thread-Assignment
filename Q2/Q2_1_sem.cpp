#include <iostream>
#include <thread>
#include <semaphore.h>
using namespace std;

sem_t sem;

void runMeFirst()
{
    cout << "Run me first." << endl;
    sem_post(&sem);
}

void runMeSecond()
{
    sem_wait(&sem);
    cout << "I run second." << endl;
}

int main()
{
    sem_init(&sem, 0, 0);

    thread first(runMeFirst);
    thread second(runMeSecond);

    first.join();
    second.join();

    sem_destroy(&sem);
    return 0;
}