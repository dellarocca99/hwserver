#ifndef __AFTTRANSACTIONSTARTCOMMAND_H__
#define __AFTTRANSACTIONSTARTCOMMAND_H__

#include<abstractions/ISASCommand.hpp>
#include <dto/CommandDTO.hpp>

class AFTTransactionStartCommand : public ISASCommand {
    oatpp::Int64 _cashamnt;
    oatpp::Int64 _resamnt;
    oatpp::Boolean _hcectrl;
    oatpp::Boolean _hcshena;
    oatpp::Boolean _hcshmode;
    oatpp::Int64 _nresamnt;
    oatpp::UInt8 _poolid;
    oatpp::UInt8 _rcptst;
    oatpp::UInt8 _trxst;

public:
    AFTTransactionStartCommand(oatpp::Object<CommandDTO> dto);
    oatpp::Object<CommandExecutionDTO> execute();
};

#endif // __AFTTRANSACTIONSTARTCOMMAND_H__