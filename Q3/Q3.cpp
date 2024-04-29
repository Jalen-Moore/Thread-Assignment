#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <cstdlib>
#include <ctime>
using namespace std;

struct requestStructure {
    int thread_id;
    int request_id;
    string ip_address;
    string page_requested;
};

string webPages[10] = {"google.com", "yahoo.com", "bing.com", "youtube.com", "wikipedia.com", 
                       "linkedin.com", "amazon.com", "ebay.com", "twitter.com", "instagram.com",};

int numRequests;
int requestCounter = 0;
mutex mtx;
condition_variable cv;
queue<requestStructure> msg_queue;

void listen(int thread_id)
{
    this_thread::sleep_for(chrono::seconds((rand() % 3) + 1));
    {
        lock_guard<mutex> lock(mtx);
        requestStructure request {thread_id, ++requestCounter, "", webPages[rand() % 10]};
        msg_queue.push(request);
    }
    cv.notify_one();
}

void do_request()
{
    while (true)
    {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, []{return !msg_queue.empty();});

        requestStructure request = msg_queue.front();
        msg_queue.pop();

        cout << "Thread " << request.thread_id << " completed request " << request.request_id << " requesting webpage " << request.page_requested << "." << endl;
        lock.unlock();
        if (request.request_id == numRequests)
            break;
    }
}

int main()
{
    srand(time(0));

    cout << "How many requests do you want to make: ";
    cin >> numRequests;

    thread request(do_request);
    thread listen_threads[numRequests];
    for (int i = 0; i < numRequests; i++)
    {
        listen_threads[i] = thread(listen, i);
    }

    request.join();
    for (int i = 0; i < numRequests; i++)
    {
        listen_threads[i].join();
    }

    return 0;
}