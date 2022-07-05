#include "SASOperation.hpp"

SASOperation::SASOperation(int id, int priority){
    _id = id;
    _priority = priority;
    _executed = false;
}

int SASOperation::getId() {
    return _id;
}

int SASOperation::getPriority() {
    return _priority;
}

void SASOperation::awaitExecution() {
    std::unique_lock<std::mutex> lk(_m_execution);
    _cv_execution.wait(lk, [this]{ return isExecuted(); });
}

bool SASOperation::isExecuted() {
    return _executed;
}

void SASOperation::execute () { 
    internalExecute();

    std::lock_guard<std::mutex> lk(_m_execution);
    _executed = true;
    _cv_execution.notify_all();
}