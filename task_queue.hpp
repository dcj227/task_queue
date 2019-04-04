#ifndef __DC_TASK_QUEUE_H__
#define __DC_TASK_QUEUE_H__

#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>
#include <queue>

template <typename T>
class DcTaskQueue {
public:
    DcTaskQueue() {
        pthread_mutex_init(&queue_mutex_, NULL);
    }

    virtual ~DcTaskQueue() {
        pthread_mutex_destroy(&queue_mutex_);
    }

    void Enqueue(T* t) {
        pthread_mutex_lock(&queue_mutex_);
        queue_.push(t);
        size_++;
        pthread_mutex_unlock(&queue_mutex_);
    }

    T* Dequeue() {
        pthread_mutex_lock(&queue_mutex_);
        if (queue_.empty()) {
            pthread_mutex_unlock(&queue_mutex_);
            return NULL;
        }
        T* tmp = queue_.front();
        queue_.pop();
        size_--;
        pthread_mutex_unlock(&queue_mutex_);
        return tmp;
    }

    uint32_t Size() {
        return size_; 
    }

    bool Empty() {
        return size_ == 0;
    }
    
private:
    std::queue<T*> queue_;
    volatile uint32_t size_;
    pthread_mutex_t queue_mutex_;
};

#endif  //  __DC_TASK_QUEUE_H__
