#include "headers/EnableCommand.hpp"

EnableCommand::EnableCommand(oatpp::Object<CommandDTO> dto) 
{

}


oatpp::Object<CommandExecutionDTO> EnableCommand::execute() 
{
#if defined(NO_SAS)
    const bool fEnabled = true;
#else
    auto eCom = sasEngineManagerService->getEngine();
    const bool fEnabled = eCom->sendReady();
#endif // NO_SAS

    sasEngineManagerService->setInitialized(fEnabled);


    auto response = CommandExecutionDTO::createShared();

    response->success = fEnabled;
    response->result = nullptr;

    return response;
}
