#ifndef __COMMANDDTO_H__
#define __COMMANDDTO_H__

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

class CommandDTO : public oatpp::DTO {
  DTO_INIT(CommandDTO, DTO)
  
  DTO_FIELD(String, name);
  DTO_FIELD(Any, parameters);
  
};

#include OATPP_CODEGEN_END(DTO)

#endif // __COMMANDDTO_H__