#ifndef __DISCONNECT_H__
#define __DISCONNECT_H__

#include "SASOperation.hpp"
#include <services/LoggingService.hpp>

#include <sasapi.h>


class Disconnect : SASOperation {
    private:
        std::shared_ptr<EngineAPI::EngineCom> _engine;

        void internalExecute();
    public:
        Disconnect(int id, int priority = 0);
};

#endif // __DISCONNECT_H__