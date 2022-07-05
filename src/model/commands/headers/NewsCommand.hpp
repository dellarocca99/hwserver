#ifndef __NEWSCOMMAND_H__
#define __NEWSCOMMAND_H__

#include <abstractions/ISASCommand.hpp>
#include <dto/CommandDTO.hpp>
#include <dto/CommandExecutionDTO.hpp>


class NewsCommand : public ISASCommand {  
public:
    NewsCommand(oatpp::Object<CommandDTO> dto);
    oatpp::Object<CommandExecutionDTO> execute();
};


#endif // __NEWSCOMMAND_H__