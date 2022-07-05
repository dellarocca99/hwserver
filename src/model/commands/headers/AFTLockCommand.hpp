#ifndef __AFTLOCKCOMMAND_H__
#define __AFTLOCKCOMMAND_H__

#include<abstractions/ISASCommand.hpp>
#include <dto/CommandDTO.hpp>
#include <dto/CommandsDTO.hpp>
#include <sasapi.h>

class AFTLockCommand : public ISASCommand {
    AFTLockDTO::Wrapper _parameters; 
public:
    AFTLockCommand(oatpp::Object<CommandDTO> dto);
    oatpp::Object<CommandExecutionDTO> execute();
};

#endif // __AFTLOCKCOMMAND_H__