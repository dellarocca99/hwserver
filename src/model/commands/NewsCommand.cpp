#include "headers/NewsCommand.hpp"

NewsCommand::NewsCommand(oatpp::Object<CommandDTO> dto) : ISASCommand()
{
    
}

oatpp::Object<CommandExecutionDTO> NewsCommand::execute() 
{
    loggingService->info("Se ejecuta el comando NEWS");

    auto result = CommandExecutionDTO::createShared();

    auto cmd = sasEngineManagerService->popCommand();

    result->success = cmd != nullptr;
    result->result = cmd;
    
    return result; 
}

