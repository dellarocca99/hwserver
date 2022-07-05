#ifndef __SASOPERATION_H__
#define __SASOPERATION_H__

#include <mutex>
#include <condition_variable>

#include <services/ServerConfigurationService.hpp>
#include <services/LoggingService.hpp>

class SASOperation {
    protected:
        OATPP_COMPONENT(std::shared_ptr<ServerConfigurationService>, serverConfigurationService);
        OATPP_COMPONENT(std::shared_ptr<LoggingService>, loggingService);

        int _priority;
        int _id;
        bool _executed;

        std::mutex _m_execution;
        std::condition_variable _cv_execution;

        virtual void internalExecute() = 0;
    public:
        SASOperation(int id, int priority = 0);

        int getId();
        int getPriority();
        void awaitExecution();
        bool isExecuted();

        void execute();
};

#endif // __SASOPERATION_H__