#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool ready = false;
bool finished = false;

void countUp()
{
    std::unique_lock<std::mutex> lock(mtx);
    while (!ready)
    {
        cv.wait(lock);
    }

    for (int i = 1; i <= 20; ++i)
    {
        std::cout << "Counting up: " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    finished = true;
    cv.notify_all();
}

void countDown()
{
    std::unique_lock<std::mutex> lock(mtx);
    while (!ready)
    {
        cv.wait(lock);
    }

    for (int i = 20; i >= 0; --i)
    {
        std::cout << "Counting down: " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    finished = true;
    cv.notify_all();
}

int main()
{
    std::thread t1(countUp);
    std::thread t2(countDown);

    {
        std::lock_guard<std::mutex> lock(mtx);
        ready = true;
    }

    cv.notify_all();

    t1.join();
    t2.join();

    std::cout << "Counting finished." << std::endl;

    return 0;
}
