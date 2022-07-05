#ifndef __SASENGINEGAMETESTTHREAD_H__
#define __SASENGINEGAMETESTTHREAD_H__

#include "SASCycledThread.hpp"

#include <services/LoggingService.hpp>
#include <services/ServerConfigurationService.hpp>

#include <sasapi.h>

class SASEngineGameTestThread : public SASCycledThread {
    public:
        SASEngineGameTestThread(std::shared_ptr<EngineAPI::EngineCom> engine, uint32_t juego);
        SASEngineGameTestThread(std::shared_ptr<EngineAPI::EngineCom> engine, uint32_t juego, const std::chrono::milliseconds & interval);
        ~SASEngineGameTestThread();

        void run ();
    private:
        OATPP_COMPONENT(std::shared_ptr<ServerConfigurationService>, serverConfigurationService);
        OATPP_COMPONENT(std::shared_ptr<LoggingService>, loggingService);

        std::shared_ptr<EngineAPI::EngineCom> _engine;
        uint32_t _juego{0};
        uint64_t toCredits(const uint64_t _val, const uint8_t _deno);
};

#endif // __SASENGINEGAMETESTTHREAD_H__