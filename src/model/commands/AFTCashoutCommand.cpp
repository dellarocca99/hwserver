#include "headers/AFTCashoutCommand.hpp"

AFTCashoutCommand::AFTCashoutCommand(oatpp::Object<CommandDTO> dto)
{
}

oatpp::Object<CommandExecutionDTO> AFTCashoutCommand::execute()
{
	EngineAPI::AFT aft;
	auto eCom = sasEngineManagerService->getEngine();

	aft.init(eCom);

	auto result = CommandExecutionDTO::createShared();

	result->success = aft.requestHostCashout(AFT_RQSTHOSTCO);
	result->result = nullptr;

	return result;

}
