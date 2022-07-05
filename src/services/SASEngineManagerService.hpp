#ifndef __SASENGINEMANAGERSERVICE_H__
#define __SASENGINEMANAGERSERVICE_H__



#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>

#include <queue>
#include <thread>
#include <mutex>

/*
#include <dto/ConfigurationDTO.hpp>

#include <services/LoggingService.hpp>
#include <utils/threads/SASEngineGameTestThread.hpp>

*/

#include <utils/threads/SASCycledThread.hpp>
#include <sasapi.h>
#include <dto/ConfigurationDTO.hpp>
#include <dto/CommandDTO.hpp>

class SASEngineManagerService {
        bool _initialized = false;
        EngineAPI::TITO* _tito;
        EngineAPI::AFT* _aft;
        ticket_t _currentTicket;
        ConfigDTO::Wrapper _config;
        std::shared_ptr<SASCycledThread> _pollThread;

        
        std::queue<CommandDTO::Wrapper> _queue;
        std::mutex _m_queue;
    public:
        ~SASEngineManagerService();


        void setEngine(std::shared_ptr<EngineAPI::EngineCom> engine);
        std::shared_ptr<EngineAPI::EngineCom> getEngine();

        void setInitialized(bool initialized);
        bool isInitialized();

        void resetTito();
        void finishTito();
        EngineAPI::TITO* getCurrentTito();

        void setCurrentTicket(const ticket_t &currentTicket);
        ticket_t getCurrentTicket();

        void setConfig(ConfigDTO::Wrapper config);
        ConfigDTO::Wrapper getConfig();

        void pushCommand(CommandDTO::Wrapper dto);
        CommandDTO::Wrapper popCommand();

        
        EngineAPI::AFT* getCurrentAft();
        void resetAft();
        void finishAft();

        void stopThread();
        void startThread();
    /*
        bool connect();
        void fillAFTStatus(EngineAPI::AFT &aft);
        void disconnect();

        void setSASConfiguration(const oatpp::Object<SASConfigDTO>& configDTO);
        void setMachineConfiguration(const oatpp::Object<SASMachineConfigDTO>& machineConfigDTO);
        void setGamesConfiguration(const oatpp::List<oatpp::Object<SASGameConfigDTO>>& gamesConfigDTO);
        oatpp::Object<SASConfigDTO> getSASConfiguration();
        oatpp::Object<SASMachineConfigDTO> getMachineConfiguration();
        oatpp::List<oatpp::Object<SASGameConfigDTO>> getGamesConfiguration();

        void addBill(int countryCode, int denoCode, int billMeter, int creditsToAdd);
        void addCredits(int value);

        void startPlay(int coinIn, int gameId);
        void endPlay(bool win, int coinOut, int gameId);

        void startGameTestThread();
        void stopGameTestThread();*/

    private:

        std::shared_ptr<EngineAPI::EngineCom> _engine;
        //std::shared_ptr<SASEngineGameTestThread> _gameThread;
};

#endif // __SASENGINEMANAGERSERVICE_H__