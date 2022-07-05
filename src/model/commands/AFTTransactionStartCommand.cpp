#include "headers/AFTTransactionStartCommand.hpp"


AFTTransactionStartCommand::AFTTransactionStartCommand(oatpp::Object<CommandDTO> dto) 
{
    auto mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
	auto aux = mapper->writeToString(dto->parameters);
	auto parameters = mapper->readFromString<oatpp::Object<AFTTransactionStartDTO>>(aux);
    
    _cashamnt = parameters->cashamnt;
    _resamnt =  parameters->resamnt;
    _hcectrl =  parameters->hcectrl;
    _hcshena = parameters->hcshena;
    _hcshmode = parameters->hcshmode;
    _nresamnt = parameters->nresamnt;
    _poolid = parameters->poolid;
    _rcptst = parameters->rcptst;
    _trxst = parameters->trxst;
}


oatpp::Object<CommandExecutionDTO> AFTTransactionStartCommand::execute() 
{
    auto pAFT = sasEngineManagerService->getCurrentAft();

    if (_cashamnt != nullptr)
        pAFT->transfer.setCasheableAmnt(*_cashamnt);
    if (_resamnt != nullptr)
        pAFT->transfer.setRestAmnt(*_resamnt);
    if (_hcectrl != nullptr)
        pAFT->transfer.setCashoutToHostControl(*_hcectrl);
    if (_hcshena != nullptr)
        pAFT->transfer.setCashoutToHost(*_hcshena);
    if (_hcshmode != nullptr)
        pAFT->transfer.setHostCashoutMode(*_hcshmode);
    if (_nresamnt != nullptr)
        pAFT->transfer.setNonRestAmnt(*_nresamnt);
    if (_poolid != nullptr)
        pAFT->transfer.setPoolID(*_poolid);
    if (_rcptst != nullptr)
        pAFT->transfer.setReceiptStatus(*_rcptst);
    if (_trxst != nullptr)
        pAFT->transfer.setStatus(*_trxst);


    auto result = CommandExecutionDTO::createShared();

    result->success = pAFT->updateAFTTrStatus();
    result->result = nullptr;

    return result;
}
