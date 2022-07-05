#ifndef __INITCOMMAND_H__
#define __INITCOMMAND_H__

#include<abstractions/ISASCommand.hpp>
#include <dto/CommandDTO.hpp>
#include <sasapi.h>

class InitCommand : public ISASCommand {
    EngineAPI::configSE_t _cfg;

public:
    InitCommand(oatpp::Object<CommandDTO> dto);
    oatpp::Object<CommandExecutionDTO> execute();
};

#endif // __INITCOMMAND_H__