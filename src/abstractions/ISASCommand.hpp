#ifndef __ISASCOMMAND_H__
#define __ISASCOMMAND_H__

#include <services/LoggingService.hpp>
#include <services/SASEngineManagerService.hpp>
#include <dto/CommandExecutionDTO.hpp>
#include <dto/CommandsDTO.hpp>

#include <oatpp/parser/json/mapping/ObjectMapper.hpp>

class ISASCommand {
protected:
    OATPP_COMPONENT(std::shared_ptr<LoggingService>, loggingService);
    OATPP_COMPONENT(std::shared_ptr<SASEngineManagerService>, sasEngineManagerService);
public:
    ISASCommand(){}
    virtual ~ISASCommand(){}

    virtual oatpp::Object<CommandExecutionDTO> execute() = 0;
};
#endif // __ISASCOMMAND_H__