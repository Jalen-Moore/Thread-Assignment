#include <iostream>
#include <thread>
#include <semaphore.h>
using namespace std;

sem_t sem1;
sem_t sem2;

void runMeFirst()
{
    cout << "Run me first." << endl;
    sem_post(&sem1);
}

void runMeSecond()
{
    sem_wait(&sem1);
    cout << "I run second." << endl;
    sem_post(&sem2);
}

void runMeThird()
{
    sem_wait(&sem2);
    cout << "I run third." << endl;
}

int main()
{
    sem_init(&sem1, 0, 0);
    sem_init(&sem2, 0, 0);

    thread first(runMeFirst);
    thread second(runMeSecond);
    thread third(runMeThird);

    first.join();
    second.join();
    third.join();

    sem_destroy(&sem1);
    sem_destroy(&sem2);
    return 0;
}