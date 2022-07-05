#include "headers/TicketOutValidationRecordCommand.hpp"

TicketOutValidationRecordCommand::TicketOutValidationRecordCommand(oatpp::Object<CommandDTO> dto) 
{
    auto configDTO = sasEngineManagerService->getConfig();
    auto mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
    auto aux = mapper->writeToString(dto->parameters);
    auto parameters = mapper->readFromString<oatpp::Object<TicketOutValidationRecordDTO>>(aux);
    
    _type = parameters->type;
    _amnt = parameters->amnt;
    _tktNbr = parameters->tktnbr;
    
    auto valid = parameters->valid;
    auto exp = parameters->exp;

    int ix = 0;
    for (auto b : *valid){
        _valNbr[ix] = b;
        ix++;
    }

    for (auto b : *exp) 
        _sExp.emplace_back(b);
}


oatpp::Object<CommandExecutionDTO> TicketOutValidationRecordCommand::execute() 
{
    bool ret = false;

    auto pTito = sasEngineManagerService->getCurrentTito();

    ret = pTito->sendValidationRecord(
        _type,
        _amnt,
        _tktNbr,
        _valNbr,
        _sExp
    );

    sasEngineManagerService->finishTito();

    auto result = CommandExecutionDTO::createShared();
    
    result->success = ret;
    result->result = nullptr;
    return result;
}
