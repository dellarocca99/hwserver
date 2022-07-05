#ifndef __SASENGINEPOLLTHREAD_H__
#define __SASENGINEPOLLTHREAD_H__

#include "SASCycledThread.hpp"
#include <services/LoggingService.hpp>
#include <services/SASEngineManagerService.hpp>

#include <sasapi.h>
#include <chrono>


class SASEnginePollThread : public SASCycledThread{
    public:
        SASEnginePollThread(const std::chrono::milliseconds& interval = std::chrono::milliseconds(250));
        void run();
    private:
        OATPP_COMPONENT(std::shared_ptr<LoggingService>, loggingService);
        OATPP_COMPONENT(std::shared_ptr<SASEngineManagerService>, sasEngineManagerService);

        void procROMSig(const pcCmd& cmd);
		void procLockPlay(const pcCmd& cmd);
        void procEnablePlay(const pcCmd& cmd);
        void procEnableBillAccp(const pcCmd& cmd);
        void procDisableBillAccp(const pcCmd&cmd);
        void procConfigBills(const pcCmd& cmd);
        void procEnaJkptHPReset(const pcCmd& cmd);
        void procEnaAutoRebet(const pcCmd& cmd);
        void procEnaDisGame(const pcCmd& cmd);
        void procExtValidationSt(const pcCmd& cmd);
        void procTicketData(const pcCmd& cmd);
        void procAFTRegFin(const pcCmd& cmd);
        void procAFTRegCancel(const pcCmd& cmd);
        void procAFTOperACK(const pcCmd& cmd);
        void procAFTLockReq(const pcCmd& cmd);
        void procAFTInitTrx(const pcCmd& cmd);
        void procAFTTicketData(const pcCmd& cmd);
        void procAFTCustTktData(const pcCmd& cmd);
};

#endif // __SASENGINEPOLLTHREAD_H__