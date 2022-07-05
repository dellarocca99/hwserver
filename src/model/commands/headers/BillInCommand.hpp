#ifndef __BILLINCOMMAND_H__
#define __BILLINCOMMAND_H__

#include<abstractions/ISASCommand.hpp>
#include <dto/CommandDTO.hpp>

class BillInCommand : public ISASCommand {
    uint8_t _ctry;
    uint8_t _deno;
    uint32_t _mtrId;
public:
    BillInCommand(oatpp::Object<CommandDTO> dto);
    oatpp::Object<CommandExecutionDTO> execute();
};
#endif // __BILLINCOMMAND_H__