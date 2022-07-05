#include "SASCommunicationService.hpp"
#include <boost/lexical_cast.hpp>  

SASCommunicationService::SASCommunicationService () : _thread(&SASCommunicationService::run, this) {
    _run = true;
}

SASCommunicationService::~SASCommunicationService() {
    stopWorking();
    _thread.join();
}

void SASCommunicationService::run () {
    loggingService->info("SASCommunicationService starting SASCommunication thread");

    while (keepWorking()) {
        loggingService->info("SASCommunicationService working...");
        std::unique_lock<std::mutex> lck(_m_empty);
        _cv_empty.wait(lck, [this] {return operationAvailable() || !keepWorking();});

        if (!keepWorking()) continue;

        loggingService->info("SASCommunicationService operation is available");

        std::lock_guard<std::mutex> guard(_m_operations);

        auto operation = _operations.front();
        _operations.pop();

        executeOperation(operation);
    }
    
    loggingService->info("SASCommunicationService executing last operations");

    if (operationAvailable()) {
        std::lock_guard<std::mutex> guard(_m_operations);
        for (auto operation = _operations.front(); !_operations.empty(); _operations.pop())
            executeOperation(operation);
    }

    loggingService->info("SASCommunicationService finishing SASCommunication thread");
}

bool SASCommunicationService::keepWorking() {
    std::lock_guard<std::mutex> guard(_m_run);
    return _run;
}

void SASCommunicationService::stopWorking() {
    std::lock_guard<std::mutex> guard(_m_run);
    _run = false;
    _cv_empty.notify_all();
}

bool SASCommunicationService::operationAvailable(){
    std::lock_guard<std::mutex> guard(_m_operations);
    return !_operations.empty();
}

void SASCommunicationService::executeOperation(std::shared_ptr<SASOperation> operation){
    loggingService->info("SASCommunicationService about to execute operation: " + boost::lexical_cast<std::string>(operation->getId()));
    operation->execute();
}

void SASCommunicationService::addOperation(std::shared_ptr<SASOperation> operation) {
    std::lock_guard<std::mutex> guard(_m_operations);
    
    loggingService->info("Queued tasks amount " + boost::lexical_cast<std::string>(_operations.size()));
    loggingService->info("SASCommunicationService adding operation " + boost::lexical_cast<std::string>(operation->getId()));

    _operations.push(operation);
    _cv_empty.notify_all();
}