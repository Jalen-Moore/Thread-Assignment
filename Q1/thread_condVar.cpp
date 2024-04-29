#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;

mutex mtx;                  // Declare mutex
condition_variable cv;      // Declare condition variable
bool ready = false;         // Shared flag

void print_message()
{
    unique_lock<mutex> lock(mtx);
    while (!ready)
        cv.wait(lock);

    cout << "Hello, world!" << endl;
}

void prepare_message()
{
    this_thread::sleep_for(chrono::seconds(1));
    ready = true;
    cv.notify_one();        // Notify waiting threads
}

int main()
{
    thread t1(print_message);
    thread t2(prepare_message);

    t1.join();
    t2.join();

    return 0;
}