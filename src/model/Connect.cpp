#include "Connect.hpp"

#include <thread>
#include <chrono>

using namespace std::chrono_literals;

Connect::Connect(int id, int priority) : SASOperation(id, priority)
{
    _port = serverConfigurationService->getPort();
    _seid = serverConfigurationService->getSEID();
    _cfgFile = serverConfigurationService->getSASCFGFile();
}

void Connect::internalExecute() 
{
	connect();
}

bool Connect::connect() 
{
    EngineAPI::configSE_t cfg;
    bool sal = false;
    uint32_t cnt = 0;
	uint8_t vcfg = 0;

	cfg.portNbr = _port;
	strcpy(cfg.portCfg, "115200,N,8,1,N");
	memcpy(cfg.SEID, _seid.c_str(), 3 * sizeof(uint32_t));
    
	if (!_engine->init(cfg))
	{
		loggingService->info("The engine API couldn't be initialized");
        return;
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
        return;
	}

    while (doInit)
	{
        switch (state)
        {
        case 0:     // HELLO
            if (cnt > 5)
            {
                loggingService->info("The SE didn't answer to the HELLO command");
                return;
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
}

void Connect::fillAFTStatus(EngineAPI::AFT &aft) 
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