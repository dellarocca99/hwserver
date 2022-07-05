#ifndef __GAMESTARTCOMMAND_H__
#define __GAMESTARTCOMMAND_H__

#include<abstractions/ISASCommand.hpp>
#include <dto/CommandDTO.hpp>

class GameStartCommand : public ISASCommand {
    gameStart_t _gs;
public:
    GameStartCommand(oatpp::Object<CommandDTO> dto);
    oatpp::Object<CommandExecutionDTO> execute();
};

#endif // __GAMESTARTCOMMAND_H__