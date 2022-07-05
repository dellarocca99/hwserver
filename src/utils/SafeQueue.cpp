#include "SafeQueue.hpp"


template<class T>
SafeQueue<T>::SafeQueue() 
{
}

template<class T>
void SafeQueue<T>::push(T elem) 
{
    const std::lock_guard<std::mutex> lock(_m_queue);
    _queue.push(elem);
}

template<class T>
T SafeQueue<T>::pop() 
{
    const std::lock_guard<std::mutex> lock(_m_queue);
    
    if (_queue.empty()) return nullptr;

    T& aux = _queue.front();
    _queue.pop();

    return aux;
}

template<class T>
T SafeQueue<T>::top() 
{
    const std::lock_guard<std::mutex> lock(_m_queue);

    if (_queue.empty()) return nullptr;

    return _queue.front();
}

template<class T>
int SafeQueue<T>::size() 
{
    const std::lock_guard<std::mutex> lock(_m_queue);
    _queue.size();
}
