#ifndef __HELLOWORLD_H__
#define __HELLOWORLD_H__

#include "SASOperation.hpp"
#include <services/LoggingService.hpp>

class HelloWorld : public SASOperation {
    private:
        void internalExecute();
    public:
        HelloWorld(int id, int priority);
};


#endif // __HELLOWORLD_H__