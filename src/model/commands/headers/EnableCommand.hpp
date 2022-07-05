#ifndef __ENABLECOMMAND_H__
#define __ENABLECOMMAND_H__

#include<abstractions/ISASCommand.hpp>
#include <dto/CommandDTO.hpp>

class EnableCommand : public ISASCommand {
public:
    EnableCommand(oatpp::Object<CommandDTO> dto);
    oatpp::Object<CommandExecutionDTO> execute();
};

#endif // __ENABLECOMMAND_H__