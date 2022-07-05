#include "headers/InitCommand.hpp"

#include <oatpp/parser/json/mapping/ObjectMapper.hpp>

InitCommand::InitCommand(oatpp::Object<CommandDTO> dto) 
{
    auto mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
    auto parametersStr = mapper->writeToString(dto->parameters);
    auto parameters = mapper->readFromString<InitDTO::Wrapper>(parametersStr);
    _cfg.portNbr = parameters->com;

    uint32_t seid[3];
   
    int ix = 0;
    for (auto seidVal : *(parameters->seid)) {
        if (ix == 3) break;

        seid[ix++] =* seidVal;
    }

    strcpy(_cfg.portCfg, "115200,N,8,1,N");
    memcpy(_cfg.SEID, seid, sizeof(seid));
}

oatpp::Object<CommandExecutionDTO> InitCommand::execute() 
{
    bool res = false;

#if defined(NO_SAS)
    res = true;
#else
    auto eCom = sasEngineManagerService->getEngine();
    sasEngineManagerService->stopThread();
    
    try {
        eCom.reset(new EngineAPI::EngineCom);
        res = eCom->init(_cfg);
    }
    catch (const std::exception& _ex)
    {
        throw;
    }
    if (res == true) {
        sasEngineManagerService->setEngine(eCom);
        sasEngineManagerService->startThread();
    }
#endif
	auto result = CommandExecutionDTO::createShared();
	result->success = res;
	result->result = nullptr;
	return result;
}
