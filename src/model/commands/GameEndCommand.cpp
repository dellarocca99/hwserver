#include "headers/GameEndCommand.hpp"


GameEndCommand::GameEndCommand(oatpp::Object<CommandDTO> dto) 
{
    
    auto mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
    auto aux = mapper->writeToString(dto->parameters);
    auto parameters = mapper->readFromString<oatpp::Object<GameEndDTO>>(aux);

    _ge.win = parameters->win;
}

oatpp::Object<CommandExecutionDTO> GameEndCommand::execute() 
{
#if defined(NO_SAS)
    const bool ret = true;
#else
    EngineAPI::gameMachine gm;
    auto eCom = sasEngineManagerService->getEngine();
    gm.init(eCom);
    const bool ret = gm.sendGameEnd(_ge);
#endif // NO_SAS


	auto result = CommandExecutionDTO::createShared();
	result->success = ret;
	result->result = nullptr;
	return result;
}
