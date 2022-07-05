#include "headers/GameStartCommand.hpp"


GameStartCommand::GameStartCommand(oatpp::Object<CommandDTO> dto) 
{
    auto mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
    auto aux = mapper->writeToString(dto->parameters);
    auto parameters = mapper->readFromString<oatpp::Object<GameStartDTO>>(aux);

    _gs.credWag = parameters->crbet;
    _gs.wagType = parameters->wt;
    _gs.totCoinIn = parameters->tcin;
    _gs.prgsvGrp = 0; //probablemente haya que cambiar esto
}


oatpp::Object<CommandExecutionDTO> GameStartCommand::execute() 
{

#if defined(NO_SAS)
    const bool ret = true;
#else
    EngineAPI::gameMachine gm;
    auto eCom = sasEngineManagerService->getEngine();
    gm.init(eCom);
    const bool ret = gm.sendGameStart(_gs);
#endif // NO_SAS


	auto result = CommandExecutionDTO::createShared();
	result->success = ret;
	result->result = nullptr;
	return result;
}
