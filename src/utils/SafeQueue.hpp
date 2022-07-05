#ifndef __SAFEQUEUE_H__
#define __SAFEQUEUE_H__

#include <queue>
#include <thread>
#include <mutex>

template<class T>
class SafeQueue{
    public:
    SafeQueue();
    
    void push(T);
    T pop();
    T top();

    int size();

    private:
    std::queue<T> _queue;

    std::mutex _m_queue;
};

#endif // __SAFEQUEUE_H__