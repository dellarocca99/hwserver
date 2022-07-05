#include "Disconnect.hpp"

Disconnect::Disconnect(int id, int priority) : SASOperation(id, priority)
{
    
}

void Disconnect::internalExecute() 
{
    loggingService->info("Executing disconnect operation.");
    _engine->deInit();
}