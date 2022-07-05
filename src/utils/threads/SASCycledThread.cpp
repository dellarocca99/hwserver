#include "SASCycledThread.hpp"


SASCycledThread::SASCycledThread() 
{
    _threadPtr = nullptr;
    _hasInterval = false;
    _running = false;
}

SASCycledThread::SASCycledThread(const std::chrono::milliseconds & interval) 
{
    _interval = interval;
    _hasInterval = true;
    _threadPtr = nullptr;
    _running = false;
}

SASCycledThread::~SASCycledThread(){
    stop();
    join();
}

void SASCycledThread::start() 
{
    std::lock_guard<std::mutex> guard(_m_running);
    if (!_running){
        _running = true;
        _threadPtr = std::make_shared<std::thread>(&SASCycledThread::runCycle, this);
    }
}

void SASCycledThread::stop() 
{
    std::lock_guard<std::mutex> guard(_m_running);
    _running = false;
}

void SASCycledThread::join()
{
    _threadPtr->join();
}

bool SASCycledThread::isRunning() 
{
    std::lock_guard<std::mutex> guard(_m_running);
    return _running;
}


void SASCycledThread::runCycle() 
{
    while (isRunning()){
        run();

        if (_hasInterval)
            std::this_thread::sleep_for(_interval);
    }
}