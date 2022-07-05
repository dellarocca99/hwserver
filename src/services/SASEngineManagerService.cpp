#include "SASEngineManagerService.hpp"

#include <boost/format.hpp>
#include <thread>
#include <chrono>

#include <utils/threads/SASEnginePollThread.hpp>

using namespace std::chrono_literals;



SASEngineManagerService::~SASEngineManagerService() 
{ 
    _pollThread->stop();
    _pollThread->join();
}

void SASEngineManagerService::setEngine(std::shared_ptr<EngineAPI::EngineCom> engine) 
{
    _engine = engine;
}

std::shared_ptr<EngineAPI::EngineCom> SASEngineManagerService::getEngine() 
{
    return _engine;
}

void SASEngineManagerService::setInitialized(bool initialized) 
{
    _initialized = initialized;
}

bool SASEngineManagerService::isInitialized() 
{
    return _initialized;
}

void SASEngineManagerService::resetTito() 
{
    if (_tito) delete _tito;
    _tito = new EngineAPI::TITO;
    _tito->init(_engine);
}

void SASEngineManagerService::finishTito() 
{
    if (_tito != nullptr) {
        delete _tito;        
        _tito = nullptr;
    }
}

EngineAPI::TITO* SASEngineManagerService::getCurrentTito() 
{
    return _tito;
}

void SASEngineManagerService::setCurrentTicket(const ticket_t &currentTicket) 
{
    _currentTicket = currentTicket;
}

ticket_t SASEngineManagerService::getCurrentTicket() 
{
    return _currentTicket;
}

void SASEngineManagerService::setConfig(ConfigDTO::Wrapper config) 
{
    _config = config;
}

ConfigDTO::Wrapper SASEngineManagerService::getConfig() 
{
    return _config;
}

void SASEngineManagerService::pushCommand(CommandDTO::Wrapper dto) 
{
    const std::lock_guard<std::mutex> lock(_m_queue);
    _queue.push(dto);
}

CommandDTO::Wrapper SASEngineManagerService::popCommand() 
{
    const std::lock_guard<std::mutex> lock(_m_queue);

    if (_queue.empty()) return nullptr;

    auto aux = _queue.front();
    _queue.pop();

    return aux;
}

EngineAPI::AFT* SASEngineManagerService::getCurrentAft() 
{
    return _aft;
}

void SASEngineManagerService::resetAft() 
{
    if (_aft) delete _aft;

    _aft = new EngineAPI::AFT;
    _aft->init(_engine);
}

void SASEngineManagerService::finishAft()
{
    if (_aft) delete _aft;
    _aft = nullptr;
}

void SASEngineManagerService::stopThread() 
{
    if (_pollThread != nullptr)
        _pollThread->stop();
}

void SASEngineManagerService::startThread() 
{
    if (_pollThread == nullptr)
        _pollThread = std::make_shared<SASEnginePollThread>();

    _pollThread->start();
}
/*
bool SASEngineManagerService::connect() 
{
    loggingService->info("Connecting and initializing sasapi");

    int _port = serverConfigurationService->getPort();
    char* _seid = serverConfigurationService->getSEID();
    std::string _cfgFile = serverConfigurationService->getSASCFGFile();

    EngineAPI::configSE_t cfg;
    bool sal = false;
    uint32_t cnt = 0;
	uint8_t vcfg = 0;

	cfg.portNbr = _port;
	strcpy(cfg.portCfg, "115200,N,8,1,N");
	memcpy(cfg.SEID, _seid, 3 * sizeof(uint32_t));
    
    if (!_engine)
	{
		try
		{
			_engine = std::make_shared<EngineAPI::EngineCom>();
		}
		catch (const std::exception& e)
		{
			loggingService->error((boost::format("Error creando el engine: %1%") % e.what()).str());
			return false;
		}
	}

	if (!_engine->init(cfg))
	{
		loggingService->error("The engine API couldn't be initialized");
        return false;
	}

    uint16_t crc = 0;
	bool nvOk = false;
	bool doInit = true;
	uint32_t state = 0;

    auto pCfg = EngineAPI::engConfig::init();

    if (!pCfg->load(_cfgFile.c_str()))
	{
		loggingService->info("There's no valid config");
		_engine->deInit();
        return false;
	}

    while (doInit)
	{
        switch (state)
        {
        case 0:     // HELLO
            if (cnt > 5)
            {
                loggingService->info("The SE didn't answer to the HELLO command");
                return false;
            }

            if (_engine->sendHello(vcfg, crc, nvOk))
            {
                state = 1;
            }
            else
            {
                ++cnt;
                std::this_thread::sleep_for(1000ms);
            }
            break;

        case 1:     // Analize Config
            if ((vcfg == 0) || !pCfg->verifyCRC(crc))
                state = 2;
            else if (!nvOk)
                state = 3;
            else
                state = 5;
            break;

        case 2:     // Send config
            if (pCfg->send(_engine))
            {
                state = 3;
                std::this_thread::sleep_for(1000ms);
            }
            else
            {
                loggingService->info("Failed sending config");
                doInit = false;
            }
            break;

        case 3:     // Send Reset NV
            if (_engine->sendResetNVRam())
            {
                state = 4;
            }
            else
            {
                loggingService->info("NV Ram reset failed");
                doInit = false;
            }
            break;

        case 4:     // Send Reset Hard (No way out)
            //engine.sendReset();
            loggingService->info("Turn SE Off and On again");
            doInit = false;
            break;

        case 5:     // Send Counters
            // El juego aqui debera enviar los contadores necesarios
            state = 6;
            break;

        case 6:     // Send FyH
            if (_engine->sendFyH())
            {
                state = 7;
            }
            else
            {
                loggingService->info("DateTime set failed");
                doInit = false;
            }
            break;

        case 7: // Send AFT Status
            if (pCfg->getMaq()->aftEnabled())
            {
                aftLockStat_t ls;
                EngineAPI::AFT aft;

                // Completo la info de ls
                aft.init(_engine);
                fillAFTStatus(aft);
                if (aft.updateAFTLsData())
                {
                    state = 8;
                }
                else
                {
                    loggingService->info("updateAFTLsData failed");
                    doInit = false;
                }
            }
            else
            {
                state = 8;
            }
            break;

        case 8:     // Send Ready
            // Enviamos el comando READY. Si el equipo no coincide, este
            // comando nunca recibe respuesta (porque el equipo se bloquea)
            if (_engine->sendReady())
            {
                doInit = false;
                sal = true;
            }
            else
            {
                loggingService->info("READY command failed. Please reboot SE");
                doInit = false;
            }
            break;

        default:
            ;
        }
	}

    if (!sal) _engine->deInit();
	return sal;
}

void SASEngineManagerService::fillAFTStatus(EngineAPI::AFT &aft) 
{
    aft.lock.setStatus(AFTLCK_ST_NOTLCK);

    // Available Transactions -----------------------------------
    // Estado actual de la GM para realizar transferencias
    uint8_t mask = 0;

    // Verificar si la máquina puede subir dinero o no, si puede
    // se marca este campo
    mask |= EAVT_TRXTOGMOK;

	// Verificar si la máquina puede realizar pagos y si puede
    // utilizar el host como medio de pago
    mask |= EAVT_TRXFROMGMOK;

    // Si la maquina puede utilizar la impresora como medio de pago
    // mask |= EAVT_TRXTOPRNOK;

    // Premio esperando a ser descargado
    // mask |= EAVT_WINAMPCTH;

    // Transferencia de bonus permitida
    //mask |= EAVT_BONAWTOGMOK;
    
    aft.lock.setAvailTrx(mask);

    // Host cashout status -------------------------------------------
    // Estos bits representan el estado del host como medio de cobro

    mask = 0;

    // Quien controla la activacion / desactivacion del host como medio de pago
    // mask |= EHCS_COTOHOSTCTRLH;  // Controlado por el host
    mask |= EHCS_COTOHOSTCTRLGM;    // Controlado por el GM

    // Estado del Cashout to host
    mask |= EHCS_COTOHOSTSTENA;     // Cashout to host enabled
    // mask |= EHCS_COTOHOSTSTDIS;     // Cashout to host disabled

    // Modo de los envios de dinero al host 0 -> Soft, 1 -> Hard
    // Soft es que si se presiona cobrar y el host no responde, esta
    // se aborta a los 8 segundos y se intenta pagar mediante otro
    // medio habilitado. Hard es que si el host no responde, se bloquea la GM y
    // se sigue intentando hasta que el operador cancela mediante llave.
    mask |= EHCS_COTOHOSTMODEHD;
    // mask |= EHCS_COTOHOSTMODESF;

    aft.lock.setHostCashoutSt(mask);

    // AFT status ----------------------------------------------------

    mask = 0;

    // Si el printer esta disponible, se setea este bit
    // mask |= AFTST_PRNAVFORRCPT;

    // Si se admiten transferencias parciales, se activa este bit
    mask |= AFTST_PARTRXHOALWD;

    // Si se soportan datos de ticket custom para AFT, se activa este bit
    // mask |= AFTST_CUSTTKTSUP;

    // Si se hizo registro AFT y este fue exitoso, se setea este bit
    // mask |= AFTST_AFTREGISTERED;

    // Si se soportan transferencias In House se setea este bit
    mask |= AFTST_INHOUSEENA;

    // Si se soportan tranferencias de bonus, se setea este bit
    // mask |= AFTST_BONUSTRENA;

    // Si se soportan transferencias debit, se setea este bit
    // mask |= AFTST_DEBITTRENA;

    aft.lock.setAFTStatus(mask);

    // Monto actual que se pueda cobrar (creditos casheables) en centavos
    aft.lock.setCurrCashAmnt(13000);

    // Monto actual en creditos restricted, en centavos
    aft.lock.setCurrRestAmnt(0);

    // Monto actual en creditos non restricted, en centavos
    aft.lock.setCurrNonRestAmnt(0);

    // Limite maximo que se puede transferir a los creditos (en centavos)
    aft.lock.setGMTransferLimit(3000000);

    // Si los creditos restricted son != 0
    if (false)
    {
	    // Expiracion de creditos restricted, si estos no son 0
        const uint8_t expi[] = {0x00, 0x00, 0x00, 0x10};
        aft.lock.setRestrictedExp(expi);

        // Pool ID de los creditos restricted, si estos no son 0
		aft.lock.setRestPoolID(0);
    }
}


void SASEngineManagerService::disconnect(){
    loggingService->info("Disconnecting sasapi");
    _engine->deInit();
}

void SASEngineManagerService::setSASConfiguration(const oatpp::Object<SASConfigDTO>& configDTO) 
{
    auto pCfg = EngineAPI::engConfig::init();

    auto pSAS = pCfg->getSAS();
    pSAS->setNumChannels(configDTO->numChannels);
    pSAS->setID(configDTO->id);
    pSAS->setNumCtrDigits(configDTO->numCtrDigits);
    pSAS->setLegacyHandPay(configDTO->legacyHandPay);

    setMachineConfiguration(configDTO->machineConfig);
    setGamesConfiguration(configDTO->gamesConfig);
}

void SASEngineManagerService::setMachineConfiguration(const oatpp::Object<SASMachineConfigDTO>& machineConfigDTO) 
{   
    auto pCfg = EngineAPI::engConfig::init();

    auto pMac = pCfg->getMaq();
    pMac->setTokenDeno(machineConfigDTO->tokenDeno);
    pMac->setSerialNumber(machineConfigDTO->serialNumber->std_str());
    pMac->setMachineFeatures(machineConfigDTO->machineFeatures->at(0).get());

}

void SASEngineManagerService::setGamesConfiguration(const oatpp::List<oatpp::Object<SASGameConfigDTO>>& gamesConfigDTO) 
{
    auto pCfg = EngineAPI::engConfig::init();
    auto pGames = pCfg->getGame();

    for (auto gameDTO : *gamesConfigDTO) {
        EngineAPI::gameObj pGame = pGames->addGame();

        pGame.setID(gameDTO->id->std_str());
        pGame.setAdditionalGameID(gameDTO->additionalGameId->std_str());
        pGame.setDenomination(gameDTO->denomination);
        pGame.setMaxBet(gameDTO->maxBet);
        pGame.setProgressiveGrp(gameDTO->progressiveGrp);
        pGame.setProgressiveLevels(std::bitset<32>(gameDTO->progressiveLevels->std_str()));
        pGame.setPayTableID(gameDTO->payTableId->std_str());
        pGame.setPayTableName(gameDTO->payTableName->std_str());
        pGame.setName(gameDTO->name->std_str());
        pGame.setBasePercent(gameDTO->basePercent);
        pGame.setWagerCategories(gameDTO->wagerCategories);
        pGame.enable(gameDTO->enable);
    }
}

oatpp::Object<SASConfigDTO> SASEngineManagerService::getSASConfiguration() 
{
    auto pCfg = EngineAPI::engConfig::init();
    auto pSAS = pCfg->getSAS();

    auto dto = SASConfigDTO::createShared();
    dto->id = pSAS->getID();
    dto->numChannels = pSAS->getNumChannels();
    dto->numCtrDigits = pSAS->getNumCtrDigits();
    dto->legacyHandPay = pSAS->getLegacyHandPay();

    dto->machineConfig = getMachineConfiguration();
    dto->gamesConfig = getGamesConfiguration();

    return dto;
}

oatpp::Object<SASMachineConfigDTO> SASEngineManagerService::getMachineConfiguration() 
{
    auto pCfg = EngineAPI::engConfig::init();
    auto pMac = pCfg->getMaq();

    auto dto = SASMachineConfigDTO::createShared();
    dto->tokenDeno = pMac->getTokenDeno();
    dto->serialNumber = pMac->getSerialNumber().c_str();


    dto->machineFeatures = oatpp::Vector<oatpp::UInt8>::createShared();

    for (auto val : std::vector(pMac->getMachineFeatures()[0], pMac->getMachineFeatures()[2]))
        dto->machineFeatures->push_back(val);

    return dto;
}

oatpp::List<oatpp::Object<SASGameConfigDTO>> SASEngineManagerService::getGamesConfiguration() 
{
    auto pCfg = EngineAPI::engConfig::init();
    auto pGames = pCfg->getGame();

    auto listDTO = oatpp::List<oatpp::Object<SASGameConfigDTO>>::createShared();
    
    
    for (int i = 0; i < pGames->getNumGames(); i++)
    {
        EngineAPI::gameObj pGame = pGames->getGame(i);
        auto dto = SASGameConfigDTO::createShared();

        dto->id = pGame.getID().c_str();
        dto->additionalGameId = pGame.getAdditionalGameID().c_str();
        dto->denomination = pGame.getDenomination();
        dto->maxBet = pGame.getMaxBet();
        dto->progressiveGrp = pGame.getProgressiveGrp();
        dto->progressiveLevels = pGame.getProgressiveLevels().to_string().c_str();
        dto->payTableId = pGame.getPayTableID().c_str();
        dto->payTableName = pGame.getPayTableName().c_str();
        dto->name = pGame.getName().c_str();
        dto->basePercent = pGame.getBasePercent();
        dto->wagerCategories = pGame.getWagerCategories();
        dto->enable = pGame.getEnabled();

        listDTO->push_back(dto);
    }

    return listDTO;
}

void SASEngineManagerService::addBill(int countryCode, int denoCode, int billMeter, int creditsToAdd) 
{
    /*
    const auto pAcc = EngineAPI::accounting::init(_engine);
    const auto pCfg = EngineAPI::engConfig::init();
    
	uint64_t cc = pCtrs->getCounter(0, CURRENT_CRED);
    const uint64_t numBills = pCtrs->getCounter(0, TOTAL_100DBILLS);
    const uint64_t td = pCtrs->getCounter(0, TOTAL_DROP);

    pAcc->setCounter(0, CURRENT_CRED, cc + value);



    std::vector<uint8_t> ctrs;
    ctrs.push_back(CURRENT_CRED);
    ctrs.push_back(TOTAL_DROP);
    ctrs.push_back(TOTAL_100DBILLS);
}

void SASEngineManagerService::addCredits(int value) 
{
}

void SASEngineManagerService::startPlay(int coinIn, int gameId) 
{
}

void SASEngineManagerService::endPlay(bool win, int coinOut, int gameId) 
{
    
}

void SASEngineManagerService::startGameTestThread() 
{
    if (_gameThread == nullptr) {
        loggingService->info("Starting game test thread");
        _gameThread = std::make_shared<SASEngineGameTestThread>(_engine, 1, 3000ms);
        _gameThread ->start();
    } else if (!_gameThread->isRunning()){
        loggingService->info("Re-starting game test thread");
        _gameThread->start();
    } else {
        loggingService->info("Game test thread is already running");
    }
}

void SASEngineManagerService::stopGameTestThread() 
{
    if (_gameThread != nullptr) {
        if (_gameThread->isRunning()){
            loggingService->info("Stopping game test thread");
            _gameThread->stop();
        } else 
            loggingService->info("Game thread is not running");
        
        _gameThread = nullptr;
    }
}*/