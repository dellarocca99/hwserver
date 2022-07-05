#ifndef __SASCOMMUNICATIONSERVICE_H__
#define __SASCOMMUNICATIONSERVICE_H__

#include "ServerConfigurationService.hpp"
#include "LoggingService.hpp"
#include <model/SASOperation.hpp>

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

class SASCommunicationService {
    private:
        OATPP_COMPONENT(std::shared_ptr<ServerConfigurationService>, serverConfigurationService);
        OATPP_COMPONENT(std::shared_ptr<LoggingService>, loggingService);

        std::queue<std::shared_ptr<SASOperation>> _operations;

        std::mutex _m_operations;
        std::mutex _m_empty;
        std::mutex _m_run;

        std::condition_variable _cv_empty;
        std::thread _thread;

        bool _run;

        bool keepWorking();
    public:
        SASCommunicationService();
        ~SASCommunicationService();

        void executeOperation(std::shared_ptr<SASOperation> operation);
        void addOperation(std::shared_ptr<SASOperation> operation);
        bool operationAvailable();
        void run ();
        void stopWorking();
};

#endif // __SASCOMMUNICATIONSERVICE_H__