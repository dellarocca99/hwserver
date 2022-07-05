#include "headers/GameSelectedCommand.hpp"

GameSelectedCommand::GameSelectedCommand(oatpp::Object<CommandDTO> dto) 
{	
	auto mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
	auto parameters = mapper->writeToString(dto->parameters);
	auto gameSelectedDTO = mapper->readFromString<oatpp::Object<GameSelectedDTO>>(parameters);

	_gameNumber = gameSelectedDTO->ng;
}

oatpp::Object<CommandExecutionDTO> GameSelectedCommand::execute() 
{
	bool ret = false;
#if defined(NO_SAS)
	ret = true;
#else
	EngineAPI::gameMachine gm;
	auto eCom = sasEngineManagerService->getEngine();

	gm.init(eCom);
	ret = gm.sendSelectedGame(_gameNumber);
#endif

	auto result = CommandExecutionDTO::createShared();
	result->success = ret;
	result->result = nullptr;
	return result;
}
