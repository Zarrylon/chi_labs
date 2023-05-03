#include <thread>
#include <mutex>
#include <atomic>
#include <random>
#include "MessageQueue.h"
#include "QueueAnalyzer.h"

std::mutex m;

template<typename T>
void* messagePushOrPop(MessageQueue<T>& message, std::chrono::system_clock::time_point start_time, std::atomic<bool>& flag)
{
    std::cout << "PushPop" << std::endl;
    
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(1, 10);

    while (flag)
    {
        auto rand = distribution(generator);
        std::this_thread::sleep_for(std::chrono::seconds(rand));

        std::unique_lock<std::mutex> lock(m);
        if (std::chrono::system_clock::now() - start_time > std::chrono::minutes(5))
        {
            flag = false;
            return nullptr;
        }

        std::cout << "Rn: " << rand << std::endl;
        if (rand % 2 == 0)
        {
            auto exp_time = std::chrono::system_clock::now();
            message.push({ rand, exp_time + std::chrono::seconds(rand), "Message" });

            std::cout << "Thread1-3: Message added" << std::endl;
        }
        else
        {
            message.pop();
            std::cout << "Thread1-3: Message deleted" << std::endl;
        }

        lock.unlock();
    }
}

template<typename T>
void* messagePop(MessageQueue<T>& message, std::chrono::system_clock::time_point start_time, std::atomic<bool>& flag)
{
    std::cout << "Pop" << std::endl;

    while (flag)
    {
        std::unique_lock<std::mutex> lock(m);

        if (std::chrono::system_clock::now() - start_time > std::chrono::minutes(5))
        {
            flag = false;
            return nullptr;
        }

        message.pop();
        std::cout << "Thread4: Message deleted" << std::endl;
        lock.unlock();

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

template<typename T>
void* messageAnalyzer(MessageQueue<T>& message, QueueAnalyzer<T>& analyzer, 
    std::chrono::system_clock::time_point start_time, std::atomic<bool>& flag, std::atomic<int>& counter)
{
    std::cout << "Analyzer" << std::endl;
    auto time = std::chrono::system_clock::now();
    
    // will be useful for preventing full queue from spamming to file
    bool isFullFirst = true;

    while (flag)
    {
        std::unique_lock<std::mutex> lock(m);
        if (std::chrono::system_clock::now() - start_time > std::chrono::minutes(5))
        {
            flag = false;
            return nullptr;
        }

        if (message.isFull())
        {
            if (isFullFirst)
            {
                std::cout << "Queue is full" << std::endl;
                std::cout << "Thread5: Analyzer called" << std::endl;

                counter++;
                isFullFirst = false;
                time = std::chrono::system_clock::now();
                analyzer.Analyzer("output.txt");
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
        else isFullFirst = true;

        if (std::chrono::system_clock::now() - time > std::chrono::minutes(1))
        {
            std::cout << "Thread5: Analyzer called" << std::endl;
            
            counter++;
            time = std::chrono::system_clock::now();
            analyzer.Analyzer("output.txt");
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        lock.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main()
{
	MessageQueue<std::string> Test(3);
	QueueAnalyzer<std::string> t(Test);
	auto start = std::chrono::system_clock::now();
    std::atomic<bool> flag(true);
    std::atomic<int> counter(0);

    std::vector<std::thread> threads;
    threads.emplace_back(messagePushOrPop<std::string>, std::ref(Test), start, std::ref(flag));
    threads.emplace_back(messagePushOrPop<std::string>, std::ref(Test), start, std::ref(flag));
    threads.emplace_back(messagePushOrPop<std::string>, std::ref(Test), start, std::ref(flag));
    threads.emplace_back(messagePop<std::string>, std::ref(Test), start, std::ref(flag));
    threads.emplace_back(messageAnalyzer<std::string>, std::ref(Test), std::ref(t),start, std::ref(flag), std::ref(counter));

    for (auto& thread : threads) {
        thread.join();
    }
    
    std::cout << std::endl << "-------------" << std::endl;
    std::cout << "Analyzer was called: " << counter << " times";
    std::cout << std::endl << "-------------" << std::endl;
}