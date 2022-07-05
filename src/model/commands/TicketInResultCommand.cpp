#include "headers/TicketInResultCommand.hpp"


TicketInResultCommand::TicketInResultCommand(oatpp::Object<CommandDTO> dto) 
{
    auto mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
    auto aux = mapper->writeToString(dto->parameters);
    auto parameters = mapper->readFromString<oatpp::Object<TicketInResultDTO>>(aux);

    _st = parameters->st; 
}

oatpp::Object<CommandExecutionDTO> TicketInResultCommand::execute() 
{
    //TODO tiene que haber una operacion tito en curso sasEngineManagerService->isTitoInProgress()
    bool ret = false;
    
    auto pTito = sasEngineManagerService->getCurrentTito();
    auto ticket = sasEngineManagerService->getCurrentTicket();

    ticket.status = _st;
    ret = pTito->ticketRedeemed(ticket);

    sasEngineManagerService->finishTito();

	auto result = CommandExecutionDTO::createShared();
	result->success = ret;
	result->result = nullptr;
	return result;
}
