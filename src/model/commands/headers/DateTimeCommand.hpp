#ifndef __DATETIMECOMMAND_H__
#define __DATETIMECOMMAND_H__

#include<abstractions/ISASCommand.hpp>
#include <dto/CommandDTO.hpp>

class DateTimeCommand : public ISASCommand {
public:
    DateTimeCommand(oatpp::Object<CommandDTO> dto);
    oatpp::Object<CommandExecutionDTO> execute();
};

#endif // __DATETIMECOMMAND_H__