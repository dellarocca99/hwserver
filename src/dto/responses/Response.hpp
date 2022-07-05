#ifndef __RESPONSE_H__
#define __RESPONSE_H__

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

class Response : public oatpp::DTO {
    DTO_INIT(Response, DTO)

    DTO_FIELD(String, status);
    DTO_FIELD(String, message);
    DTO_FIELD(Any, data);
};

class ResponseOk : public Response {
    DTO_INIT(ResponseOk, Response);
};

class ResponseFail : public Response {
    DTO_INIT(ResponseFail, Response)

    DTO_FIELD(String, reason);
};

class ResponseError : public Response {
    DTO_INIT(ResponseError, Response)

    DTO_FIELD(String, error);
};


#include OATPP_CODEGEN_END(DTO)

#endif // __RESPONSE_H__