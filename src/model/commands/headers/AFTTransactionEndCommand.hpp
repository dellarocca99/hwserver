#ifndef __AFTTRANSACTIONENDCOMMAND_H__
#define __AFTTRANSACTIONENDCOMMAND_H__

#include<abstractions/ISASCommand.hpp>
#include <dto/CommandDTO.hpp>

class AFTTransactionEndCommand : public ISASCommand {
public:
    AFTTransactionEndCommand(oatpp::Object<CommandDTO> dto);
    oatpp::Object<CommandExecutionDTO> execute();
};

#endif // __AFTTRANSACTIONENDCOMMAND_H__