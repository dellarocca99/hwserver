#ifndef __EXCEPTIONCOMMAND_H__
#define __EXCEPTIONCOMMAND_H__

#include<abstractions/ISASCommand.hpp>
#include <dto/CommandDTO.hpp>

class ExceptionCommand : public ISASCommand {
    int64_t _exception;
public:
    ExceptionCommand(oatpp::Object<CommandDTO> dto);
    oatpp::Object<CommandExecutionDTO> execute();
};

#endif // __EXCEPTIONCOMMAND_H__