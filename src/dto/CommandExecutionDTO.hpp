#ifndef __COMMANDEXECUTIONDTO_H__
#define __COMMANDEXECUTIONDTO_H__

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

class CommandExecutionDTO : public oatpp::DTO {
  DTO_INIT(CommandExecutionDTO, DTO)
  
  DTO_FIELD(Boolean, success);
  DTO_FIELD(Any, result);
  
};

#include OATPP_CODEGEN_END(DTO)

#endif // __COMMANDEXECUTIONDTO_H__