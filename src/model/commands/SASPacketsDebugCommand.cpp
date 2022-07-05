#include "headers/SASPacketsDebugCommand.hpp"
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>

SASPacketsDebugCommand::SASPacketsDebugCommand(oatpp::Object<CommandDTO> dto)
{
  auto mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
	auto aux = mapper->writeToString(dto->parameters);
	auto parameters = mapper->readFromString<oatpp::Object<EnableDebugDTO>>(aux);
	_enabled = parameters->ena;
}

oatpp::Object<CommandExecutionDTO> SASPacketsDebugCommand::execute()
{
#if defined(NO_SAS)
	const bool ret = true;
#else
	auto eCom = sasEngineManagerService->getEngine();
	const bool ret = eCom->protocolDebugger(_enabled);
#endif

	auto result = CommandExecutionDTO::createShared();

	result->success = ret;
	result->result = nullptr;

	return result;
}
