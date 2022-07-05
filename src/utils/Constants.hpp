#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#include <list>
#include <utility>

enum OperationCodes {

};

enum ErrorCodes {

};

static class Errors {
    public: 
        inline std::list<std::pair<ErrorCodes, std::string>> ErrorsList = std::list<std::pair<ErrorCodes, std::string>>(
            std::pair<ErrorCodes, std::string>()
        );
};

enum BillDenominations {
};


#endif // __CONSTANTS_H__