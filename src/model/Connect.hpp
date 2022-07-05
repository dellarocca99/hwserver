#ifndef __CONNECT_H__
#define __CONNECT_H__

#include "SASOperation.hpp"
#include <services/ServerConfigurationService.hpp>
#include <services/LoggingService.hpp>

#include <sasapi.h>


class Connect : SASOperation {
    private:
        OATPP_COMPONENT(std::shared_ptr<ServerConfigurationService>, serverConfigurationService);
        int _port;
        std::string _seid;
        std::string _cfgFile;
        std::shared_ptr<EngineAPI::EngineCom> _engine;

        void internalExecute();

        bool connect();
        void fillAFTStatus(EngineAPI::AFT &aft);

    public:
        Connect(int id, int priority = 0);
};

#endif // __CONNECT_H__