#include "headers/TicketInStatusCommand.hpp"


TicketInStatusCommand::TicketInStatusCommand(oatpp::Object<CommandDTO> dto) 
{

}

oatpp::Object<CommandExecutionDTO> TicketInStatusCommand::execute() 
{
    bool ret = false;
    bool valid = false;

    auto pTito = sasEngineManagerService->getCurrentTito();
    ticket_t ticket;
    ret = pTito->getRedemptionStatus(ticket, valid);

    sasEngineManagerService->setCurrentTicket(ticket);

    auto result = CommandExecutionDTO::createShared();
    auto resultData = oatpp::UnorderedFields<oatpp::Any>::createShared();

    resultData["valid"] = oatpp::Boolean(valid);
    resultData["amnt"] = oatpp::Int64(valid ? ticket.amount : 0LL);
    resultData["txcode"] = oatpp::Int64(ticket.txCode);
    
    result->success = ret;
    result->result = resultData;
    
    return result;
}
