#ifndef __CONFIGCOMMAND_H__
#define __CONFIGCOMMAND_H__

#include <sasapi.h>
#include <abstractions/ISASCommand.hpp>

#include <dto/CommandDTO.hpp>
#include <dto/ConfigurationDTO.hpp>

class ConfigCommand : public ISASCommand {

    ConfigDTO::Wrapper _configDTO;

    void loadSASCfg(EngineAPI::engConfig* _pCfg);
    void loadMachineCfg(EngineAPI::engConfig* _pCfg);

public:
    ConfigCommand(oatpp::Object<CommandDTO> dto);
    oatpp::Object<CommandExecutionDTO> execute();
};

#endif // __CONFIGCOMMAND_H__