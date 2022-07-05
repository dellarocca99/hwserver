#ifndef __REQUEST_H__
#define __REQUEST_H__

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

class Request : public oatpp::DTO {
  DTO_INIT(Request, DTO)
};

#include OATPP_CODEGEN_END(DTO)

#endif // __REQUEST_H__