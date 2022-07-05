#include "HelloWorld.hpp"

#include <boost/lexical_cast.hpp>

HelloWorld::HelloWorld(int id, int priority) : SASOperation(id, priority)
{
}

void HelloWorld::internalExecute() 
{
    loggingService->info("Executing operation " + boost::lexical_cast<std::string>(this->getId()));
}