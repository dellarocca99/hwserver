#include "SASEnginePollThread.hpp"

#include <boost/format.hpp>
#include <dto/CommandDTO.hpp>

SASEnginePollThread::SASEnginePollThread(const std::chrono::milliseconds& interval) 
    : SASCycledThread(interval) {
}

void SASEnginePollThread::run() 
{
    auto enginePtr = sasEngineManagerService->getEngine();
    
    if (!sasEngineManagerService->isInitialized()) {
        loggingService->info("Poll thread not initialized");
        return;
    }
    
    loggingService->info("Poll thread running new cycle");

    pcCmd cmd;

	if (enginePtr->sendQuery(cmd))
    {
//        LOG_DEBUG("SAS Core 0 Online->%u", cmd.onLine[0]);
//        LOG_DEBUG("Puede imprimir tickets->%u", cmd.puedeImprimir);

        loggingService->info("New message received" + cmd.cmd);
        switch (cmd.cmd)
        {
		case CMD_ROMSIG:        procROMSig(cmd);            break;
		case CMD_LOCKPLAY:      procLockPlay(cmd);          break;
        case CMD_ENABLEPLAY:    procEnablePlay(cmd);        break;
        case CMD_ENABILLACCP:   procEnableBillAccp(cmd);    break;
        case CMD_DISBILLACCP:   procDisableBillAccp(cmd);   break;
        case CMD_CFGBILLS:      procConfigBills(cmd);       break;
        case CMD_ENAJKPHPRST:   procEnaJkptHPReset(cmd);    break;
        case CMD_ENAAUTORBET:   procEnaAutoRebet(cmd);      break;
        case CMD_ENADISGAME:    procEnaDisGame(cmd);        break;
        case CMD_EXTVALST:      procExtValidationSt(cmd);   break;
        case CMD_TKTDATA:       procTicketData(cmd);        break;
        case CMD_AFTREGFIN:     procAFTRegFin(cmd);         break;
        case CMD_AFTREGCNLD:    procAFTRegCancel(cmd);      break;
        case CMD_AFTOPERACK:    procAFTOperACK(cmd);        break;
        case CMD_AFTLOCKREQ:    procAFTLockReq(cmd);        break;
        case CMD_AFTINITRX:     procAFTInitTrx(cmd);        break;
        case CMD_AFTTKTDATA:    procAFTTicketData(cmd);     break;
        case CMD_AFTCTKTDATA:   procAFTCustTktData(cmd);    break;
        default:                                            break;
        }
    }
}

void SASEnginePollThread::procROMSig(const pcCmd& cmd) 
{
    auto result = CommandDTO::createShared();
    auto parameters = oatpp::UnorderedFields<oatpp::Any>::createShared();

    parameters["seed"] = oatpp::UInt16(cmd.data.romSig.seed);

    result->name = "ROMSIG";
    result->parameters = parameters;

    sasEngineManagerService->pushCommand(result);
}

void SASEnginePollThread::procLockPlay(const pcCmd& cmd) 
{
    auto result = CommandDTO::createShared();

    result->name = "LOCKPLAY";
    result->parameters = nullptr;

    sasEngineManagerService->pushCommand(result);
}

void SASEnginePollThread::procEnablePlay(const pcCmd& cmd) 
{
    auto result = CommandDTO::createShared();

    result->name = "ENAPLAY";
    result->parameters = nullptr;

    sasEngineManagerService->pushCommand(result);
}

void SASEnginePollThread::procEnableBillAccp(const pcCmd& cmd) 
{
    
    auto result = CommandDTO::createShared();

    result->name = "ENABILLAC";
    result->parameters = nullptr;

    sasEngineManagerService->pushCommand(result);
}

void SASEnginePollThread::procDisableBillAccp(const pcCmd&cmd) 
{
    auto result = CommandDTO::createShared();

    result->name = "DISBILLAC";
    result->parameters = nullptr;

    sasEngineManagerService->pushCommand(result);
}

void SASEnginePollThread::procConfigBills(const pcCmd& cmd) 
{
    auto result = CommandDTO::createShared();
    auto parameters = oatpp::UnorderedFields<oatpp::Any>::createShared();
    auto bills = oatpp::List<oatpp::Boolean>::createShared();

    parameters["bamode"] = oatpp::UInt8(cmd.data.cfgBills.billAccptMode);

    for (int i = 0; i < 24; i++) 
        bills->emplace_back(oatpp::Boolean(cmd.data.cfgBills.bills[i] ? true : false));


    parameters["bills"] = bills;

    result->name = "CFGBILLS";
    result->parameters = parameters;

    sasEngineManagerService->pushCommand(result);
}

void SASEnginePollThread::procEnaJkptHPReset(const pcCmd& cmd) 
{
	auto result = CommandDTO::createShared();
    auto parameters = oatpp::UnorderedFields<oatpp::Any>::createShared();

    parameters["method"] = oatpp::UInt8(cmd.data.jackpotHPResetMethod.method);

    result->name = "JKPHPRM";
    result->parameters = parameters;

    sasEngineManagerService->pushCommand(result);
}

void SASEnginePollThread::procEnaAutoRebet(const pcCmd& cmd) 
{
	auto result = CommandDTO::createShared();
    auto parameters = oatpp::UnorderedFields<oatpp::Any>::createShared();

    parameters["enable"] = oatpp::Boolean(cmd.data.autoReBet.enable ? true : false);

    result->name = "AUTOREBET";
    result->parameters = parameters;

    sasEngineManagerService->pushCommand(result);
}

void SASEnginePollThread::procEnaDisGame(const pcCmd& cmd) 
{
	auto result = CommandDTO::createShared();
    auto parameters = oatpp::UnorderedFields<oatpp::Any>::createShared();

    parameters["gn"] = oatpp::UInt8(cmd.data.enaDisGame.gameNbr);
    parameters["enable"] = oatpp::Boolean(cmd.data.enaDisGame.enable ? true : false);
    parameters["deno"] = oatpp::UInt8(cmd.data.enaDisGame.denom);

    result->name = "ENADISGAME";
    result->parameters = parameters;

    sasEngineManagerService->pushCommand(result);
}

void SASEnginePollThread::procExtValidationSt(const pcCmd& cmd) 
{
	auto result = CommandDTO::createShared();
    auto parameters = oatpp::UnorderedFields<oatpp::Any>::createShared();

    parameters["prnascsh"] = oatpp::Boolean((cmd.data.extValStatus.stBitCtrl & EXTVAL_PRNASCSHDEV) ? true : false);
    parameters["prnashpr"] = oatpp::Boolean((cmd.data.extValStatus.stBitCtrl & EXTVAL_PRNASHPDEV) ? true : false);
    parameters["valhphpr"] = oatpp::Boolean((cmd.data.extValStatus.stBitCtrl & EXTVAL_VALHPANDHPR) ? true : false);
    parameters["ptrestkt"] = oatpp::Boolean((cmd.data.extValStatus.stBitCtrl & EXTVAL_PRNRESTKT) ? true : false);
    parameters["tkfframt"] = oatpp::Boolean((cmd.data.extValStatus.stBitCtrl & EXTVAL_TKTFFRESAMT) ? true : false);
    parameters["tktred"] =  oatpp::Boolean((cmd.data.extValStatus.stBitCtrl & EXTVAL_TKTREDEMPT) ? true : false);
    parameters["expdays"] = oatpp::UInt16(cmd.data.extValStatus.cashTktRecExp);
    parameters["defexpres"] = oatpp::UInt16(cmd.data.extValStatus.resTktdefExp);

    result->name = "EXTVALST";
    result->parameters = parameters;

    sasEngineManagerService->pushCommand(result);
}

void SASEnginePollThread::procTicketData(const pcCmd& cmd) 
{
    auto result = CommandDTO::createShared();
    auto parameters = oatpp::UnorderedFields<oatpp::Any>::createShared();

    parameters["hostid"] = oatpp::UInt16(cmd.data.ticketData.hostID);
    parameters["expdays"] = oatpp::UInt8(cmd.data.ticketData.expiration);
    parameters["location"] = oatpp::String(cmd.data.ticketData.location);
    parameters["addr1"] = oatpp::String(cmd.data.ticketData.addr1);
    parameters["addr2"] = oatpp::String(cmd.data.ticketData.addr2);
    parameters["restktit"] = oatpp::String(cmd.data.ticketData.resTktTitle);
    parameters["debtktit"] = oatpp::String(cmd.data.ticketData.debTktTit);

    result->name = "TKTDATA";
    result->parameters = parameters;

    sasEngineManagerService->pushCommand(result);
}

void SASEnginePollThread::procAFTRegFin(const pcCmd& cmd) 
{
    const aftRegData_t* pReg = &cmd.data.aftRegData;

    auto result = CommandDTO::createShared();
    auto parameters = oatpp::UnorderedFields<oatpp::Any>::createShared();
    
    parameters["regst"] = oatpp::UInt8(pReg->status);
    parameters["posid"] = oatpp::UInt32(pReg->posId);

    std::string aux;
    for (auto c : pReg->regKey)
    {
        char bcd[4];

        sprintf(bcd, "%02X ", c);
        aux += bcd;
    }
    
    parameters["regkey"] = oatpp::String(aux.c_str());

    result->name = "AFTREGEND";
    result->parameters = parameters;

    sasEngineManagerService->pushCommand(result);
}

void SASEnginePollThread::procAFTRegCancel(const pcCmd& cmd) 
{
    auto result = CommandDTO::createShared();

    result->name = "AFTREGCNL";
    result->parameters = nullptr;

    sasEngineManagerService->pushCommand(result);
}

void SASEnginePollThread::procAFTOperACK(const pcCmd& cmd) 
{
    auto result = CommandDTO::createShared();

    result->name = "AFTOPACK";
    result->parameters = nullptr;

    sasEngineManagerService->pushCommand(result);
}

void SASEnginePollThread::procAFTLockReq(const pcCmd& cmd) 
{
    EngineAPI::AFT aft;

    aft.lock.setRequest(cmd.data.aftLockReq);

    auto result = CommandDTO::createShared();
    auto parameters = oatpp::UnorderedFields<oatpp::Any>::createShared();

    parameters["oper"] = oatpp::UInt32(aft.lock.getCode());
    parameters["time"] = oatpp::UInt32(aft.lock.getTimeout());
    parameters["trxgmav"] = oatpp::Boolean(aft.lock.getTrToGMOK());
    parameters["trxhtav"] = oatpp::Boolean(aft.lock.getTrFromGMOK());
    parameters["trxprnav"] = oatpp::Boolean(aft.lock.getTrToPrnOK());
    parameters["trxbaav"] = oatpp::Boolean(aft.lock.getBonusAwToGMOK());

    result->name = "AFTLOCK";
    result->parameters = parameters;

    sasEngineManagerService->pushCommand(result);
}

void SASEnginePollThread::procAFTInitTrx(const pcCmd& cmd) 
{
    sasEngineManagerService->resetAft();
    auto pAFT = sasEngineManagerService->getCurrentAft();
    pAFT->transfer.setCurrent(cmd.data.aftTrx);    


    auto result = CommandDTO::createShared();
    auto parameters = oatpp::UnorderedFields<oatpp::Any>::createShared();

    parameters["trxcode"] = oatpp::UInt8(pAFT->transfer.getCode());
    parameters["trxtype"] = oatpp::UInt8(pAFT->transfer.getTransferType());
    parameters["cashamnt"] = oatpp::UInt64(pAFT->transfer.getReqCasheableAmnt());

    // Como no voy a emular restricted, estos no los uso
    // cmd["resamnt"];
    // cmd["nresamnt"];
    // cmd["poolid"];
    //cmd["expir"] = picojson::value(pAFT->transfer.getExpirationDate());

    parameters["hcectrl"] = oatpp::Boolean(pAFT->transfer.hostCashoutEnableCtrl());
    parameters["hcshena"] = oatpp::Boolean(pAFT->transfer.hostCashoutEnable());
    parameters["hcshmode"] = oatpp::UInt8(pAFT->transfer.hostCashoutModeReq());
    parameters["cshfrgm"] = oatpp::Boolean(pAFT->transfer.cashoutFromGM());
    parameters["lckatrx"] = oatpp::Boolean(pAFT->transfer.lockAfterTransfer());
    parameters["custktd"] = oatpp::Boolean(pAFT->transfer.useCustTktData());
    parameters["atrxoil"] = oatpp::Boolean(pAFT->transfer.acceptTrOnlyIfLocked());
    parameters["trxrcpt"] = oatpp::Boolean(pAFT->transfer.printAsked());
    parameters["trxid"] = oatpp::String(pAFT->transfer.getTransactionID().c_str());

    std::vector<EngineAPI::custRcptData_t> rcpData;

    auto cusRcptData = oatpp::List<oatpp::Any>::createShared();

    pAFT->transfer.getCustomReceiptData(rcpData);

    for (const auto& elem : rcpData)
    {
        auto rcpElem = oatpp::UnorderedFields<oatpp::Any>::createShared();

        rcpElem["field"] = oatpp::Int64(static_cast<int64_t>(elem.field));
        rcpElem["value"] = oatpp::String(elem.value.c_str());
        cusRcptData->emplace_back(rcpElem);
    }

    parameters["rcptdata"] = cusRcptData;
    parameters["lockto"] = oatpp::UInt32(pAFT->transfer.getLockTimeout());

    result->name = "AFTTRXINI";
    result->parameters = parameters;

    sasEngineManagerService->pushCommand(result);
}

void SASEnginePollThread::procAFTTicketData(const pcCmd& cmd) 
{
    EngineAPI::AFT aft;
    std::vector<EngineAPI::aftTicketItem> items;

    aft.ticketData.setDataCmd(cmd.data.aftTktData);
    aft.ticketData.getData(items);

    auto result = CommandDTO::createShared();
    auto parameters = oatpp::UnorderedFields<oatpp::Any>::createShared();

    auto rcpItems = oatpp::List<oatpp::Any>::createShared();

    for (auto item : items) {
        auto rcpItem = oatpp::UnorderedFields<oatpp::Any>::createShared();
        rcpItem["item"] = oatpp::Int64(item.code);
        rcpItem["value"] = oatpp::String(item.value.c_str());

        rcpItems->emplace_back(rcpItem);
    }

    parameters["items"] = rcpItems;

    result->name = "AFTTKTDATA";
    result->parameters = parameters;

    sasEngineManagerService->pushCommand(result);
}

void SASEnginePollThread::procAFTCustTktData(const pcCmd& cmd) 
{
    EngineAPI::AFT aft;
    std::vector<EngineAPI::aftTicketItem> items;
    int64_t oper;
    
    auto result = CommandDTO::createShared();
    auto parameters = oatpp::UnorderedFields<oatpp::Any>::createShared();

    aft.custTicketData.setDataCmd(cmd.data.aftTktData);

    if (aft.custTicketData.isSet()) {
        oper = 0LL;
        
        aft.custTicketData.getData(items);

        auto custItems = oatpp::List<oatpp::Any>::createShared();

        for (const auto& item : items)
        {
            auto custItem = oatpp::UnorderedFields<oatpp::Any>::createShared();

            custItem["item"] = oatpp::Int64(item.code);
            custItem["value"] = oatpp::String(item.value.c_str());
            custItems->emplace_back(custItem);
        }

        parameters["items"] = custItems;

    } else oper = 1LL;

    parameters["oper"] = oatpp::Int64(oper);

    result->name = "AFTCUSTKD";
    result->parameters = parameters;

    sasEngineManagerService->pushCommand(result);
}
