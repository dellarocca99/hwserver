#include "headers/AFTLockCommand.hpp"

#include <sasapi.h>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>


AFTLockCommand::AFTLockCommand(oatpp::Object<CommandDTO> dto) 
{
    auto mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
    auto parameters = mapper->writeToString(dto->parameters);
    _parameters = mapper->readFromString<oatpp::Object<AFTLockDTO>>(parameters);
}

oatpp::Object<CommandExecutionDTO> AFTLockCommand::execute() 
{

    auto pCfg = EngineAPI::engConfig::init();    
    auto eCom = sasEngineManagerService->getEngine();
    bool ret = false;

    if (pCfg->getMaq()->aftEnabled())
    {
        aftLockStat_t ls;
        EngineAPI::AFT aft;
        uint8_t val;

        // Completo la info de ls
        aft.init(eCom);

        aft.lock.setStatus(_parameters->lockst);
        aft.lock.setAvailTrx(_parameters->avtrx);

        aft.lock.setHostCashoutSt(_parameters->hostcs);

        aft.lock.setAFTStatus(_parameters->aftst);

        aft.lock.setCurrCashAmnt(_parameters->cashamt);
        aft.lock.setCurrRestAmnt(_parameters->restamt);
        aft.lock.setCurrNonRestAmnt(_parameters->nrestamt);
        aft.lock.setGMTransferLimit(_parameters->maxtrx);

        // // Si los creditos restricted son != 0
        // if (restAmt)
        // {
        //     const auto expi = _cmd.get("expi").get<picojson::array>();

        //     // Expiracion de creditos restricted, si estos no son 0
        //     uint8_t expiA[4] = {0x00, 0x00, 0x00, 0x00};
        //     expiA[0] = expi[0].get<int64_t>();
        //     expiA[1] = expi[1].get<int64_t>();
        //     expiA[2] = expi[2].get<int64_t>();
        //     expiA[3] = expi[3].get<int64_t>();
        //     aft.lock.setRestrictedExp(expiA);

        //     // Pool ID de los creditos restricted, si estos no son 0
        //     aft.lock.setRestPoolID(static_cast<uint8_t>(_cmd.get("poolid").get<int64_t>()));
        // }

        ret = aft.updateAFTLsData();
    }
    else
    {
        ret = true;
    }

    auto response = CommandExecutionDTO::createShared();

    response->success = ret;
    response->result = nullptr;

    return response;
}
