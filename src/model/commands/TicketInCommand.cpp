#include "headers/TicketInCommand.hpp"


TicketInCommand::TicketInCommand(oatpp::Object<CommandDTO> dto) 
{
    auto mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
    auto aux = mapper->writeToString(dto->parameters);
    auto parameters = mapper->readFromString<oatpp::Object<TicketInDTO>>(aux);

    _bc = parameters->bc->std_str();
}

oatpp::Object<CommandExecutionDTO> TicketInCommand::execute() 
{
    std::vector<uint8_t> bc;
    bool ret = false;
    try
    {
        sasEngineManagerService->resetTito();
        auto pTito = sasEngineManagerService->getCurrentTito();

        EngineAPI::strToBCD(_bc.c_str(), bc);
        ret = pTito->redeemTicket(bc, 0U);
    }
    catch (const std::exception& _ex)
    {
        throw;
    }
    
	auto result = CommandExecutionDTO::createShared();
	result->success = ret;
	result->result = nullptr;
	return result;
}
