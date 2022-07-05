#include "headers/BillInCommand.hpp"

BillInCommand::BillInCommand(oatpp::Object<CommandDTO> dto) 
{
    auto mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
	auto aux = mapper->writeToString(dto->parameters);
	auto parameters = mapper->readFromString<oatpp::Object<BillInDTO>>(aux);

    _ctry = parameters->ctry;
    _deno = parameters->deno;
    _mtrId = parameters->mtrid;
}

oatpp::Object<CommandExecutionDTO> BillInCommand::execute() 
{
    lastAccBill lb;

#if defined(NO_SAS)
    const bool ret = true;
#else
    auto eCom = sasEngineManagerService->getEngine();
    const auto ac = EngineAPI::accounting::init(eCom);

    lb.billMeter = ac->getCounters()->getCounter(0, _mtrId);
    lb.countryCode = _ctry;
    lb.denoCode = _deno;
    const bool ret = ac->sendBill(lb);
#endif // NO_SAS
    
    auto response = CommandExecutionDTO::createShared();

    response->success = ret;
    response->result = nullptr;

    return response;
}
