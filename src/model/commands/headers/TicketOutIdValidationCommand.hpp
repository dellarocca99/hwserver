#ifndef __TICKETOUTIDVALIDATIONCOMMAND_H__
#define __TICKETOUTIDVALIDATIONCOMMAND_H__


#include<abstractions/ISASCommand.hpp>
#include <dto/CommandDTO.hpp>

class TicketOutIdValidationCommand : public ISASCommand {
    uint8_t _type;
    uint32_t _amnt;
    uint8_t _valType;
public:
    TicketOutIdValidationCommand(oatpp::Object<CommandDTO> dto);
    oatpp::Object<CommandExecutionDTO> execute();
};
#endif // __TICKETOUTIDVALIDATIONCOMMAND_H__