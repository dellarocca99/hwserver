#ifndef __GAMEENDCOMMAND_H__
#define __GAMEENDCOMMAND_H__

#include<abstractions/ISASCommand.hpp>
#include <dto/CommandDTO.hpp>

class GameEndCommand : public ISASCommand {
    gameEnd_t _ge;
public:
    GameEndCommand(oatpp::Object<CommandDTO> dto);
    oatpp::Object<CommandExecutionDTO> execute();
};

#endif // __GAMEENDCOMMAND_H__