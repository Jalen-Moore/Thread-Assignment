#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
using namespace std;

int counter = 0;
const int NUM_THREADS = 4;
mutex mtx;
condition_variable cv;

void worker(int id)
{
    cout << "Thread " << id << " started." << endl;
    this_thread::sleep_for(chrono::seconds(1));
    unique_lock<mutex> lock(mtx);
    counter++;

    cout << "Thread " << id << " reached barrier (" << counter << "/" << NUM_THREADS << ")." << endl;
    if (counter == NUM_THREADS)
        cv.notify_all();
    else
        cv.wait(lock);

    cout << "Thread " << id << " finished." << endl; 
}

int main()
{
    cout << "Main thread started." << endl;
    thread t1(worker, 1);
    thread t2(worker, 2);
    thread t3(worker, 3);
    thread t4(worker, 4);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    cout << "Main thread finished." << endl;
    
    return 0;
}