#include "headers/ExceptionCommand.hpp"


ExceptionCommand::ExceptionCommand(oatpp::Object<CommandDTO> dto) 
{
    
    auto mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
    auto aux = mapper->writeToString(dto->parameters);
    auto parameters = mapper->readFromString<oatpp::Object<ExceptionDTO>>(aux);

    _exception = parameters->ex;
}


oatpp::Object<CommandExecutionDTO> ExceptionCommand::execute() 
{
#if defined(NO_SAS)
    bool ret = true;
#else
    EngineAPI::gameMachine gm;
    auto eCom = sasEngineManagerService->getEngine();
    gm.init(eCom);
    bool ret = gm.sendEvt(_exception);
#endif // NO_SAS

    auto response = CommandExecutionDTO::createShared();

    response->success = ret;
    response->result = nullptr;

    return response;
}
