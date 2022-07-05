#include "headers/DateTimeCommand.hpp"


DateTimeCommand::DateTimeCommand(oatpp::Object<CommandDTO> dto) 
{

}

oatpp::Object<CommandExecutionDTO> DateTimeCommand::execute() 
{
    bool ret = false;
#if defined(NO_SAS)
    ret = true;
#else
    auto eCom = sasEngineManagerService->getEngine();
    ret = eCom->sendFyH();
#endif // NO_SAS

    auto response = CommandExecutionDTO::createShared();

    response->success = ret;
    response->result = nullptr;

    return response;
}
