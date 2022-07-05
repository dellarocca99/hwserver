#ifndef __TICKETINSTATUSCOMMAND_H__
#define __TICKETINSTATUSCOMMAND_H__


#include<abstractions/ISASCommand.hpp>
#include <dto/CommandDTO.hpp>

class TicketInStatusCommand : public ISASCommand {
public:
    TicketInStatusCommand(oatpp::Object<CommandDTO> dto);
    oatpp::Object<CommandExecutionDTO> execute();
};

#endif // __TICKETINSTATUSCOMMAND_H__