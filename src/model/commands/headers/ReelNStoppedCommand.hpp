#ifndef __REELNSTOPPEDCOMMAND_H__
#define __REELNSTOPPEDCOMMAND_H__


#include<abstractions/ISASCommand.hpp>
#include <dto/CommandDTO.hpp>

class ReelNStoppedCommand : public ISASCommand {
    uint8_t _rn;
    uint8_t _phystp;
public:
    ReelNStoppedCommand(oatpp::Object<CommandDTO> dto);
    oatpp::Object<CommandExecutionDTO> execute();
};

#endif // __REELNSTOPPEDCOMMAND_H__