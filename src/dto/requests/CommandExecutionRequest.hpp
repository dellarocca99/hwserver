#ifndef __COMMANDEXECUTIONREQUEST_H__
#define __COMMANDEXECUTIONREQUEST_H__

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

#include "Request.hpp"
#include <dto/CommandDTO>

#include OATPP_CODEGEN_BEGIN(DTO)

class CommandExecutionRequest : public Request {
  DTO_INIT(CommandExecutionRequest, Request)
  DTO_FIELD(Object<CommandDTO>, command);
};

#include OATPP_CODEGEN_END(DTO)

#endif // __COMMANDEXECUTIONREQUEST_H__