#ifndef __ISASCOMMANDFACTORYSERVICE_H__
#define __ISASCOMMANDFACTORYSERVICE_H__

#include <dto/CommandDTO.hpp>
#include "ISASCommand.hpp"

class ISASCommandFactoryService {
public:
    ISASCommandFactoryService() {}
    virtual ~ISASCommandFactoryService() {}
    virtual std::shared_ptr<ISASCommand> getCommandInstance (oatpp::Object<CommandDTO> commandDTO) = 0;

};

#endif // __ISASCOMMANDFACTORYSERVICE_H__