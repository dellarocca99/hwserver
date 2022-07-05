#include "SASEngineGameTestThread.hpp"

#include <chrono>

using namespace std::chrono_literals;

SASEngineGameTestThread::SASEngineGameTestThread(std::shared_ptr<EngineAPI::EngineCom> engine, uint32_t juego)
 : SASCycledThread()
{
    _juego = juego;
    _engine = engine;
}

SASEngineGameTestThread::SASEngineGameTestThread(std::shared_ptr<EngineAPI::EngineCom> engine, uint32_t juego, const std::chrono::milliseconds & interval)
 : SASCycledThread(interval)
{
    _juego = juego;
    _engine = engine;
}

SASEngineGameTestThread::~SASEngineGameTestThread() 
{
}

void SASEngineGameTestThread::run() 
{
    loggingService->info("Ejecutando run del game thread");

	// Simulador de juego
    auto pAcc = EngineAPI::accounting::init(_engine);
    const EngineAPI::counters* pCtrs = pAcc->getCounters();
    const auto pCfg = EngineAPI::engConfig::init();
	EngineAPI::gameMachine gm;
	bool gane = false;
	uint64_t cc = pCtrs->getCounter(0, CURRENT_CRED);
	std::vector<uint8_t> ctrs;
	uint64_t gp, gp1, tcin, tcin1;
	gameStart_t gs;
	gameEnd_t ge;

    gm.init(_engine);

	// Chequeamos el dinero disponible, si no hay mas, recargamos
    if (cc == 0ULL){
        auto pGame = pCfg->getGame()->getGame(0);
        auto cc = toCredits(10000ULL, pGame.getDenomination()); 
        std::vector<uint8_t> ctrs;
        ctrs.push_back(CURRENT_CRED);
        ctrs.push_back(TOTAL_DROP);
        ctrs.push_back(TOTAL_100DBILLS);

        pAcc->setCounter(0, CURRENT_CRED, cc);
        const uint64_t numBills = pCtrs->getCounter(0, TOTAL_100DBILLS) + 1;
        const uint64_t td = pCtrs->getCounter(0, TOTAL_DROP) + cc;
        pAcc->setCounter(0, TOTAL_100DBILLS, numBills);
        pAcc->setCounter(0, TOTAL_DROP, td);

        if (!pAcc->sendCounters(0, ctrs)) {
            loggingService->error("Error SendCounters 1");
            goto Errxit;
        }

        lastAccBill bill;
        bill.countryCode = COUNTRY_ARG;
        bill.denoCode = BILLD100;
        bill.billMeter = numBills;

        if (!pAcc->sendBill(bill)){
            loggingService->error("Error send bill");
            goto Errxit;
        }

        ctrs.clear();
    }

    
	// Apuesto 1 credito y juego
	--cc;
	tcin = pCtrs->getCounter(0, TOTAL_COIN_IN) + 1;
	tcin1 = pCtrs->getCounter(_juego, TOTAL_COIN_IN) + 1;

	if (!pAcc->sendCounter(0, TOTAL_COIN_IN, tcin)) {
        loggingService->error("Error SendCounters TOTAL_COIN_IN 1");
        goto Errxit;
    }
	if (!pAcc->sendCounter(_juego, TOTAL_COIN_IN, tcin1)) {
        loggingService->error("Error SendCounters TOTAL_COIN_IN 2");
        goto Errxit;
    }
	gs.credWag = 1;
	gs.prgsvGrp = 0;
	gs.totCoinIn = tcin;
	gs.wagType = 0; // No multi denom, no max bet
	if (!gm.sendGameStart(gs)) {
        loggingService->error("Error sendGameStart");
        goto Errxit;
    }

    gane = (rand() % 10) > 5;

	// Si gane...
	ctrs.clear();
	if (gane)
	{
		ctrs.push_back(GAMES_WON);
		ctrs.push_back(TOTAL_COIN_OUT);
		const uint64_t gw = pCtrs->getCounter(0, GAMES_WON) + 1;
		const uint64_t gw1 = pCtrs->getCounter(_juego, GAMES_WON) + 1;
		const uint64_t tcout = pCtrs->getCounter(0, TOTAL_COIN_OUT) + gane;
		const uint64_t tcout1 = pCtrs->getCounter(_juego, TOTAL_COIN_OUT) + gane;
		pAcc->setCounter(0, GAMES_WON, gw);
		pAcc->setCounter(0, TOTAL_COIN_OUT, tcout);
		pAcc->setCounter(_juego, GAMES_WON, gw1);
		pAcc->setCounter(_juego, TOTAL_COIN_OUT, tcout1);
		cc += gane;
		ge.win = gane;
	}
	else
	{
		ctrs.push_back(GAMES_LOST);
		const uint64_t gl = pCtrs->getCounter(0, GAMES_LOST) + 1;
		const uint64_t gl1 = pCtrs->getCounter(_juego, GAMES_LOST) + 1;
		pAcc->setCounter(0, GAMES_LOST, gl);
		pAcc->setCounter(_juego, GAMES_LOST, gl1);
		ge.win = 0;
	}

	ctrs.push_back(GAMES_PLAYED);
	gp = pCtrs->getCounter(0, GAMES_PLAYED) + 1;
	gp1 = pCtrs->getCounter(_juego, GAMES_PLAYED) + 1;
	pAcc->setCounter(0, GAMES_PLAYED, gp);
	pAcc->setCounter(_juego, GAMES_PLAYED, gp1);
	if (!pAcc->sendCounters(0, ctrs)) goto Errxit;
	if (!pAcc->sendCounters(_juego, ctrs)) goto Errxit;
	if (!pAcc->sendCounter(0, CURRENT_CRED, cc)) goto Errxit;

	std::this_thread::sleep_for(2000ms);
	if (!gm.sendGameEnd(ge)) goto Errxit;

	return;

    
Errxit:
	loggingService->info("Error. Game Test aborted");
    this->stop();
}

uint64_t SASEngineGameTestThread::toCredits(const uint64_t _val, const uint8_t _deno)
{
    const uint64_t divisor[] =
    {
    1ULL, 		5ULL, 		10ULL, 		25ULL, 		50ULL,
    100ULL, 	500ULL, 	1000ULL,    2000ULL,    10000ULL,
    20ULL,  	200ULL, 	250ULL,     2500ULL,    5000ULL,
    20000ULL,   25000ULL,   50000ULL,   100000ULL,  200000ULL,
    250000ULL,  500000ULL,  2ULL,       3ULL,       15ULL,
    40ULL
    };

    const uint32_t divNum = sizeof(divisor) / sizeof(divisor[0]);

    return _val / divisor[_deno - 1];
}