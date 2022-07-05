#ifndef __COMMANDSDTO_H__
#define __COMMANDSDTO_H__

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

class AFTLockDTO : public oatpp::DTO {
    DTO_INIT(AFTLockDTO, DTO)
  
    DTO_FIELD(UInt8, lockst); 
    DTO_FIELD(UInt8, avtrx); 
    DTO_FIELD(UInt8, hostcs); 
    DTO_FIELD(UInt8, aftst); 
    DTO_FIELD(UInt64, cashamt); 
    DTO_FIELD(UInt64, restamt); 
    DTO_FIELD(UInt64, nrestamt); 
    DTO_FIELD(UInt64, maxtrx);   
};

class InitDTO : public oatpp::DTO {
    DTO_INIT(InitDTO, DTO)

    DTO_FIELD(List<UInt32>, seid);
    DTO_FIELD(UInt32, com);
};

class EnableDebugDTO : public oatpp::DTO {
    DTO_INIT(EnableDebugDTO, DTO)
    
    DTO_FIELD(Boolean, ena);
};

class GameSelectedDTO : public oatpp::DTO {
    DTO_INIT(GameSelectedDTO, DTO)

    DTO_FIELD(UInt32, ng);
};

class AFTTransactionStartDTO : public oatpp::DTO {
    DTO_INIT(AFTTransactionStartDTO, DTO)
    
    DTO_FIELD(Int64, cashamnt);
    DTO_FIELD(Int64, resamnt);
    DTO_FIELD(Boolean, hcectrl);
    DTO_FIELD(Boolean, hcshena);
    DTO_FIELD(Boolean, hcshmode);
    DTO_FIELD(Int64, nresamnt);
    DTO_FIELD(UInt8, poolid);
    DTO_FIELD(UInt8, rcptst);
    DTO_FIELD(UInt8, trxst);
};

class BillInDTO : public oatpp::DTO {
    DTO_INIT(BillInDTO, DTO)

    DTO_FIELD(UInt8, ctry);
    DTO_FIELD(UInt8, deno);
    DTO_FIELD(UInt32, mtrid);
};

class ExceptionDTO : public oatpp::DTO {
    DTO_INIT(ExceptionDTO, DTO)

    DTO_FIELD(Int64, ex);
};

class GameEndDTO : public oatpp::DTO {
    DTO_INIT(GameEndDTO, DTO)

    DTO_FIELD(UInt32, win);
};

class GameStartDTO : public oatpp::DTO {
    DTO_INIT(GameStartDTO, DTO)

    DTO_FIELD(UInt16, crbet);
    DTO_FIELD(UInt8, wt);
    DTO_FIELD(UInt32, tcin);
    //DTO_FIELD(UInt32, prgsvGrp);
};

class ReelNStoppedDTO : public oatpp::DTO {
    DTO_INIT(ReelNStoppedDTO, DTO)

    DTO_FIELD(UInt8, reel);
    DTO_FIELD(UInt8, phystp);
};

class TicketInDTO : public oatpp::DTO {
    DTO_INIT(TicketInDTO, DTO)

    DTO_FIELD(String, bc);
};

class TicketInResultDTO : public oatpp::DTO {
    DTO_INIT(TicketInResultDTO, DTO)

    DTO_FIELD(UInt8, st);
};

class TicketOutIdValidationDTO : public oatpp::DTO {
    DTO_INIT(TicketOutIdValidationDTO, DTO)

    DTO_FIELD(UInt8, type);
    DTO_FIELD(UInt32, amnt);
};

class TicketOutValidationRecordDTO : public oatpp::DTO {
    DTO_INIT(TicketOutValidationRecordDTO, DTO)

    DTO_FIELD(UInt8, type);
    DTO_FIELD(UInt32, amnt);
    DTO_FIELD(UInt32, tktnbr);
    DTO_FIELD(List<UInt8>, valid);
    DTO_FIELD(List<UInt8>, exp);
};


#include OATPP_CODEGEN_END(DTO)


#endif // __COMMANDSDTO_H__