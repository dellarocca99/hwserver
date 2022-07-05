#ifndef __TICKETOUTVALIDATIONRECORDCOMMAND_H__
#define __TICKETOUTVALIDATIONRECORDCOMMAND_H__

#include<abstractions/ISASCommand.hpp>
#include <dto/CommandDTO.hpp>

class TicketOutValidationRecordCommand : public ISASCommand {
    uint8_t _type;
    int64_t _amnt;
    uint16_t _tktNbr;
    EngineAPI::valNumber_t _valNbr;
    std::vector<uint8_t> _sExp;
public:
    TicketOutValidationRecordCommand(oatpp::Object<CommandDTO> dto);
    oatpp::Object<CommandExecutionDTO> execute();
};

#endif // __TICKETOUTVALIDATIONRECORDCOMMAND_H__