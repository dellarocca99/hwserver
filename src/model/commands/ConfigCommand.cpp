
#include <sasapi.h>
#include "headers/ConfigCommand.hpp"

void ConfigCommand::loadSASCfg(EngineAPI::engConfig* _pCfg)
{
    try {
        auto pSAS = _pCfg->getSAS();

        pSAS->setID(_configDTO->sas->id);
        pSAS->setNumChannels(_configDTO->sas->nc);
        pSAS->setLegacyHandPay(_configDTO->sas->lgcyhp);
    }
    catch (const std::exception& e) {
        std::string msg = "Error loading SAS configuration. ";
        std::string error = e.what();

        throw std::exception((msg + error).c_str());
    }
}

void ConfigCommand::loadMachineCfg(EngineAPI::engConfig* _pCfg)
{
    try
    {
        auto pMaq = _pCfg->getMaq();
        auto cfgMaq = _configDTO->gm;
        auto cfgTito = _configDTO->tito;
        auto cfgAFT = _configDTO->aft;

        const bool aftEna = cfgAFT->enabled;
        uint8_t valType = SAS_VAL_STD;

        if (cfgTito->secenh)
            valType = SAS_VAL_SE;
        else if (cfgTito->system)
            valType = SAS_VAL_SYS;

        const bool titoEna = (valType != 0);

        pMaq->setTokenDeno(cfgMaq->denom);
        pMaq->setSerialNumber(cfgMaq->serialnbr->c_str());

        // Armo las machine features
        uint8_t macFeat[3] = { 0, 0, 0 };

        if (cfgMaq->jkptmult) macFeat[0] |= MFEAT1_JACKPOTMULT;
        if (cfgMaq->lgcybonus) macFeat[0] |= MFEAT1_LGCYBONUSAWRD;
        if (cfgMaq->tournmt) macFeat[0] |= MFEAT1_TOURNAMENT;

        switch (cfgMaq->woncredmet)
        {
        case 0: macFeat[1] |= MFEAT2_METMODWCMWW;   break;
        case 1: macFeat[1] |= MFEAT2_METMODWCMWPP;  break;
        }

        if (cfgMaq->extmeters) macFeat[1] |= MFEAT2_EXTDMETERS;
        //if (cfgMaq.get("cmpauth").get<bool>()) macFeat[1] |= MFEAT2_COMPAUTH; // No soportado por SE
        //if (cfgMaq.get("multdeno").get<bool>()) macFeat[1] |= MFEAT2_MULTIDENOM; // No soportado por SE
        if (cfgMaq->rtesup) macFeat[2] |= MFEAT3_MAXPOLLINGRATE;
        //if (cfgMaq.get("multsasprgsv").get<bool>()) macFeat[2] |= MFEAT3_MSASPRGSVWIN; // No soportado por SE

        if (aftEna)
        {
            macFeat[1] |= MFEAT2_AFT;
            if (cfgAFT->bonus) macFeat[0] |= MFEAT1_AFTBONUSAWARD;
        }

        if (titoEna)
        {
            if (cfgTito->valext) macFeat[0] |= MFEAT1_VALIDATEXT;

            switch (valType)
            {
            case 1: macFeat[0] |= MFEAT1_VALSECENH; break;
            case 2: macFeat[0] |= MFEAT1_VALSYSTEM; break;
            }

            if (cfgTito->tktRed) macFeat[0] |= MFEAT1_TKTREDEMPT;
            if (cfgTito->tkttotdtcc) macFeat[1] |= MFEAT2_TKTTDTCC;
        }

        pMaq->setMachineFeatures(macFeat);

        // Extended validation
        const auto& extVal = cfgMaq->extval;
        pMaq->setModifStatusMask(MIM_PRNASCASHOUT, true);
        pMaq->setModifStatusMask(MIM_PRNASHPRECEP, true);
        pMaq->setModifStatusMask(MIM_VALHPANDHPR, true);
        pMaq->setModifStatusMask(MIM_PRINTRESTKT, true);
        pMaq->setModifStatusMask(MIM_TKTFORRESAMT, true);
        pMaq->setModifStatusMask(MIM_TKTREDEMPT, true);
        pMaq->setModifStatusMask(MIM_SECENHCFG, true);

        // Bits de control
        pMaq->setStatusMask(MIM_PRNASCASHOUT, extVal->prnascsh);
        pMaq->setStatusMask(MIM_PRNASHPRECEP, extVal->prnashp);
        pMaq->setStatusMask(MIM_VALHPANDHPR, extVal->valhphpr);
        pMaq->setStatusMask(MIM_PRINTRESTKT, extVal->prnrestkt);
        pMaq->setStatusMask(MIM_TKTFORRESAMT, extVal->tkfframnt);
        pMaq->setStatusMask(MIM_TKTREDEMPT, extVal->tktred);
        pMaq->setStatusMask(MIM_SECENHCFG, extVal->secencfg);

        // Dias de expiracion
        pMaq->setCashTktAndRecExp(extVal->cashTktRecExp);
        pMaq->setRestTktDefExpiration(extVal->resTktdefExp);

        // Asset Number
        pMaq->setAssetNumber(extVal->assetNbr);

        // Games
        auto games = cfgMaq->games;
        auto pGames = _pCfg->getGame();
        if (pGames->getNumGames() > 1) pGames->reset();

        int ix = 0;
        for (auto game : *games)
        {
            EngineAPI::gameObj pGame;

            if (ix)
            {
                pGame = pGames->addGame();
                pGame.enable(true);
            }
            else
            {
                pGame = pGames->getGame(0);
            }

            pGame.setID(game->gameID->std_str());
            pGame.setAdditionalGameID(game->aditionalGameID->std_str());
            pGame.setDenomination(game->denomination);
            pGame.setMaxBet(game->maxBet);
            pGame.setPayTableID(game->payTableID->std_str());

            uint16_t bp = atoi(game->basePercent->c_str());
            pGame.setBasePercent(bp);
            pGame.setName(game->name->std_str());
            pGame.setPayTableName(game->payTableName->std_str());
            pGame.setProgressiveGrp(0);

            std::bitset<32> bs;
            pGame.setProgressiveLevels(bs); // Van todos en 0

            pGame.setWagerCategories(0);
            ++ix;
        }
    }
    catch (const std::exception& e)
    {
        std::string msg = "Error loading machine configuration. ";
        std::string error = e.what();

        throw std::exception((msg + error).c_str());
    }
}

ConfigCommand::ConfigCommand(oatpp::Object<CommandDTO> dto)
{
    auto mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
    auto parameters = mapper->writeToString(dto->parameters);
    _configDTO = mapper->readFromString<oatpp::Object<ConfigDTO>>(parameters);
}



oatpp::Object<CommandExecutionDTO> ConfigCommand::execute() 
{
    bool fReboot = false, sal = false;
    auto pCfg = EngineAPI::engConfig::init();

#if defined(NO_SAS)
    sal = true;
#else
    // Seteo la config
    // SAS
    loadSASCfg(pCfg);
    // GM
    loadMachineCfg(pCfg);

    // Envio HELLO
    uint8_t validCfg;
    uint16_t crc;
    bool validNv;
    uint32_t cntr = 0;
    bool sendResetNV = false;
    auto eCom = sasEngineManagerService->getEngine();
        
    do
    {
        const bool res = eCom->sendHello(validCfg, crc, validNv);

        if (!res && cntr == 20)
            throw std::exception("Could not establish communication.");

        if (res) break;

        ++cntr;
    } while (true);

    // Vamos a chequear el estado de la config
    if ((validCfg == 0) || !pCfg->verifyCRC(crc))
    {
        // Envio la config
        if (pCfg->send(eCom))
        {
            ::Sleep(1000); //TODO NO SE POR QUE ESTA ESTO
            sendResetNV = true;
        }
        else throw std::exception("Could not update configuration.");
    }

    // Debo enviar un reset de NV?
    if (!validNv || sendResetNV)
    {
        if (eCom->sendResetNVRam() && eCom->sendReset())
        {
            fReboot = true;
            sal = true;
        }
        /*else
        {
            goto answerSal;
        }*/
    }
    else sal = true;

#endif // NO_SAS

    if (sal) sasEngineManagerService->setConfig(_configDTO);
    
    auto result = CommandExecutionDTO::createShared();
    auto resultData = oatpp::UnorderedFields<oatpp::Any>::createShared();

    resultData["reboot"] = oatpp::Boolean(fReboot);
    
    result->success = sal;
    result->result = resultData;
    return result;
}
