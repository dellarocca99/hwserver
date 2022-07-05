#ifndef __SASCYCLEDTHREAD_H__
#define __SASCYCLEDTHREAD_H__

#include <thread>
#include <mutex>

class SASCycledThread {
    public:
        SASCycledThread();
        SASCycledThread(const std::chrono::milliseconds & interval);
        virtual ~SASCycledThread();

        void start();
        void stop();
        void join();
        bool isRunning();
        virtual void run () = 0;

    private:
        std::shared_ptr<std::thread> _threadPtr;

        std::mutex _m_running;
        bool _running;

        bool _hasInterval;
        std::chrono::milliseconds _interval;

    protected:
        virtual void runCycle();
};

#endif // __SASCYCLEDTHREAD_H__