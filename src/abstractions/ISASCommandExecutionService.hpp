#ifndef __ISASCOMMANDEXECUTIONSERVICE_H__
#define __ISASCOMMANDEXECUTIONSERVICE_H__

#include <dto/CommandDTO.hpp>
#include <dto/CommandExecutionDTO.hpp>

class ISASCommandExecutionService{
public:
    ISASCommandExecutionService() {}
    virtual ~ISASCommandExecutionService() {}
    virtual std::shared_ptr<CommandExecutionDTO> executeCommand (std::shared_ptr<ISASCommand> command) = 0;
};

#endif // __ISASCOMMANDEXECUTIONSERVICE_H__