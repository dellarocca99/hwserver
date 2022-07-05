#ifndef __TICKETINRESULTCOMMAND_H__
#define __TICKETINRESULTCOMMAND_H__


#include<abstractions/ISASCommand.hpp>
#include <dto/CommandDTO.hpp>

class TicketInResultCommand : public ISASCommand {
    uint8_t _st;
public:
    TicketInResultCommand(oatpp::Object<CommandDTO> dto);
    oatpp::Object<CommandExecutionDTO> execute();
};

#endif // __TICKETINRESULTCOMMAND_H__