#include "headers/CountersCommand.hpp"

#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <sasapi.h>

CountersCommand::CountersCommand(oatpp::Object<CommandDTO> dto) 
{
    auto mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
    auto parameters = mapper->writeToString(dto->parameters);
    _counters = mapper->readFromString<oatpp::Object<CountersDTO>>(parameters);
}

void CountersCommand::updateMeters() 
{
    auto eCom = sasEngineManagerService->getEngine();
    
#if !defined(NO_SAS)
    auto pAcc = EngineAPI::accounting::init(eCom);
#endif // NO_SAS
    
    for (auto ctr : *(_counters->meters))
    {
        const uint8_t ctrId = ctr->id;        
#if !defined(NO_SAS)
        for (auto value : *(ctr->values)){ 
            pAcc->sendCounter(value->gn, ctrId, value->value);
        }
#endif // NO_SAS
    }
}

oatpp::Object<CommandExecutionDTO> CountersCommand::execute() 
{

    updateMeters();

    auto response = CommandExecutionDTO::createShared();

    response->success = true;
    response->result = nullptr;

    return response;
}
