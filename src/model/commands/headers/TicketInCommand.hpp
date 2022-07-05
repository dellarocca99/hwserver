#ifndef __TICKETINCOMMAND_H__
#define __TICKETINCOMMAND_H__


#include<abstractions/ISASCommand.hpp>
#include <dto/CommandDTO.hpp>

class TicketInCommand : public ISASCommand {
    std::string _bc;
public:
    TicketInCommand(oatpp::Object<CommandDTO> dto);
    oatpp::Object<CommandExecutionDTO> execute();
};

#endif // __TICKETINCOMMAND_H__