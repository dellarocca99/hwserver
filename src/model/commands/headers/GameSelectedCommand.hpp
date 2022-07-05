#ifndef __GAMESELECTEDCOMMAND_H__
#define __GAMESELECTEDCOMMAND_H__

#include <sasapi.h>
#include <abstractions/ISASCommand.hpp>
#include <dto/CommandDTO.hpp>

class GameSelectedCommand : public ISASCommand {
    uint32_t _gameNumber;
public:
    GameSelectedCommand(oatpp::Object<CommandDTO> dto);
    oatpp::Object<CommandExecutionDTO> execute();
};


#endif // __GAMESELECTEDCOMMAND_H__