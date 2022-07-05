#include "headers/AFTTransactionEndCommand.hpp"

AFTTransactionEndCommand::AFTTransactionEndCommand(oatpp::Object<CommandDTO> dto)
{
}

oatpp::Object<CommandExecutionDTO> AFTTransactionEndCommand::execute()
{
	auto pAFT = sasEngineManagerService->getCurrentAft();
	bool ret = pAFT->endAFTTransaction();

	sasEngineManagerService->finishAft();

	auto result = CommandExecutionDTO::createShared();

	result->success = ret;
	result->result = nullptr;

	return result;
}
