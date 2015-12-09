/*
 * CS425 Final Project
 * Matt Smith, Reuben Romandy, Maksim Gomov
 * SharedQueue.h
 * 
 * A concurrent queue implementation, used for the task queue in ParallelTravellingSalesman.
 */

#ifndef __Shared_Queue__
#define __Shared_Queue__

#include <queue>
#include <mutex>

template <typename T>
class Shared_Queue {
private:
    std::queue<T> sharedQueue;
    std::mutex queueMutex;
    
public:
    Shared_Queue() {
        
    }
    
    T pop() {
        try {
            std::lock_guard<std::mutex> lock(queueMutex);
            if (!sharedQueue.empty()) {
                T result = sharedQueue. front();
                sharedQueue.pop();
                return result;
            }
            else {
                throw std::runtime_error("Shared Queue is Empty");
            }
        }
        catch (std::runtime_error e) {
            throw e;
        }
    }
    
    void push(T input) {
        std::lock_guard<std::mutex> lock(queueMutex);
        sharedQueue.push(input);
    }
    
    bool isEmpty() {
        std::lock_guard<std::mutex> lock(queueMutex);
        return sharedQueue.empty();
    }
};

#endif /* defined(__Shared_Queue__) */
