#include <cstdio>
#include <thread>
#include <vector>
#include <mutex>

void sayHello(int i)
{
    printf("Hello from thread %ld and count %d\n", std::this_thread::get_id(), i);
}

void trueParallelism()
{
    std::vector<std::thread> threads;

    printf("True parallelism is achieved when multiple threads run on different cores.\n");
    printf("This allows for concurrent execution of tasks, improving performance and responsiveness.\n");

    for (int i = 0; i < 5; i++)
    {
        threads.emplace_back(sayHello, i);
        printf("Thread %ld is created and running...\n", threads.back().get_id());
    }

    for (auto &t : threads)
    {
        t.join(); // Wait for all threads to finish
        printf("Thread %ld has finished execution.\n", t.get_id());
    }
}

void incrementCounter(int &counter, std::mutex &mtx)
{
    for (int i = 0; i < 10000; ++i)
    {
        std::lock_guard<std::mutex> lock(mtx);
        // Lock the mutex to ensure exclusive access to the counter
        // printf("Thread %ld incrementing counter: %d\n", std::this_thread::get_id(), counter);
        // Simulate some work
        // std::this_thread::sleep_for(std::chrono::milliseconds(1));
        std::this_thread::sleep_for(std::chrono::microseconds(1)); // Simulate some work
        ++counter;
    }
}
void mutexExample()
{
    printf("Mutex example to prevent ");
    std::mutex mtx;
    int counter = 0;

    std::thread t1(incrementCounter, std::ref(counter), std::ref(mtx));
    std::thread t2(incrementCounter, std::ref(counter), std::ref(mtx));

    t1.join();
    t2.join();
    printf("Final counter value: %d\n", counter);
}

int main()
{

    std::thread thread1(sayHello, 89);
    printf("Hello from main thread %ld\n", std::this_thread::get_id());

    printf("Waiting for thread %ld to finish...\n", thread1.joinable() ? thread1.get_id() : std::thread::id());
    printf("Waiting for thread joined %b \n", thread1.joinable());

    thread1.detach(); // Detach the thread to allow it to run independently

    std::thread t1(sayHello, 89);

    for (int i = 0; i < 5; i++)
    {
        if (t1.joinable())
            t1.join(); // Join the thread if it is joinable
        t1 = std::thread(sayHello, i);
        printf("Thread %ld is created and running...\n", t1.get_id());
    }

    if (t1.joinable())
    {
        printf("Joining thread %ld...\n", t1.get_id());
        t1.join(); // Join the thread to wait for its completion
    }
    else
    {
        printf("Thread %ld is not joinable.\n", t1.get_id());
    }
    trueParallelism();
    mutexExample();

    return 0;
}