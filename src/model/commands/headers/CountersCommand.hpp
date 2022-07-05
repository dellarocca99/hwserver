#ifndef __COUNTERSCOMMAND_H__
#define __COUNTERSCOMMAND_H__

#include<abstractions/ISASCommand.hpp>
#include <dto/CommandDTO.hpp>
#include <dto/CountersDTO.hpp>

class CountersCommand : public ISASCommand {
    CountersDTO::Wrapper _counters;

public:
    CountersCommand(oatpp::Object<CommandDTO> dto);
    void updateMeters();
    oatpp::Object<CommandExecutionDTO> execute();
};

#endif // __COUNTERSCOMMAND_H__