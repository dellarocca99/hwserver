#ifndef __AFTCASHOUTCOMMAND_H__
#define __AFTCASHOUTCOMMAND_H__

#include<abstractions/ISASCommand.hpp>
#include <dto/CommandDTO.hpp>

class AFTCashoutCommand : public ISASCommand {
public:
    AFTCashoutCommand(oatpp::Object<CommandDTO> dto);
    oatpp::Object<CommandExecutionDTO> execute();
};


#endif // __AFTCASHOUTCOMMAND_H__