#include "headers/ReelNStoppedCommand.hpp"


ReelNStoppedCommand::ReelNStoppedCommand(oatpp::Object<CommandDTO> dto) 
{   
    auto mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
    auto aux = mapper->writeToString(dto->parameters);
    auto parameters = mapper->readFromString<oatpp::Object<ReelNStoppedDTO>>(aux);

    _rn = parameters->reel;
    _phystp = parameters->phystp;
}


oatpp::Object<CommandExecutionDTO> ReelNStoppedCommand::execute() 
{

#if defined(NO_SAS)
    const bool ret = true;
#else
    EngineAPI::gameMachine gm;
    auto eCom = sasEngineManagerService->getEngine();
    gm.init(eCom);
    const bool ret = gm.sendReelStop(_rn, _phystp);
#endif


    auto result = CommandExecutionDTO::createShared();

    result->success = ret;
    result->result = nullptr;
    
    return result; 
}
