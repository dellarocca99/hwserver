#ifndef __SASPACKETSDEBUGCOMMAND_H__
#define __SASPACKETSDEBUGCOMMAND_H__


#include<abstractions/ISASCommand.hpp>
#include <dto/CommandDTO.hpp>

class SASPacketsDebugCommand : public ISASCommand {
    bool _enabled; 
public:
    SASPacketsDebugCommand(oatpp::Object<CommandDTO> dto);
    oatpp::Object<CommandExecutionDTO> execute();
};

#endif // __SASPACKETSDEBUGCOMMAND_H__