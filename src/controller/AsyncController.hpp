#ifndef __ASYNCCONTROLLER_H__
#define __ASYNCCONTROLLER_H__

#include <dto/DTOs.hpp>
#include <services/LoggingService.hpp>

#include <oatpp/web/server/api/ApiController.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>

#include <thread>
#include <chrono>

using namespace std::chrono_literals;

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin codegen

class AsyncController : public oatpp::web::server::api::ApiController {
protected:
  AsyncController(const std::shared_ptr<ObjectMapper>& objectMapper)
  : oatpp::web::server::api::ApiController(objectMapper)
  {}
public:

  static std::shared_ptr<AsyncController> createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>,
                                                                      objectMapper)){
    return std::shared_ptr<AsyncController>(new AsyncController(objectMapper));
  }

  ENDPOINT_ASYNC("GET", "/", Root) {
    
    ENDPOINT_ASYNC_INIT(Root)
    
    Action act() override {
      OATPP_COMPONENT(std::shared_ptr<LoggingService>, loggingService);

      auto dto = HelloDto::createShared();
      dto->message = "Hello Async!";
      dto->server = Header::Value::SERVER;
      dto->userAgent = request->getHeader(Header::USER_AGENT);

      std::this_thread::sleep_for(300ms);

      loggingService->trace("Le pegaron al endpoint");

      auto response = controller->createDtoResponse(Status::CODE_200, dto);

      response->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
      response->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
      response->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
      //response->putHeaderIfNotExists("Connection", "close");

      return _return(response);
    }
    
  };
  
  ENDPOINT_ASYNC("GET", "/body/string", EchoStringBody) {
    
    ENDPOINT_ASYNC_INIT(EchoStringBody)
    
    Action act() override {
      return request->readBodyToStringAsync().callbackTo(&EchoStringBody::returnResponse);
    }
    
    Action returnResponse(const oatpp::String& body){
      return _return(controller->createResponse(Status::CODE_200, body));
    }
    
  };

  ENDPOINT_ASYNC("GET", "/body/dto", EchoDtoBody) {
    
    ENDPOINT_ASYNC_INIT(EchoDtoBody)
    
    Action act() override {
      return request->readBodyToDtoAsync<oatpp::Object<MessageDto>>(controller->getDefaultObjectMapper()).callbackTo(&EchoDtoBody::returnResponse);
    }
    
    Action returnResponse(const oatpp::Object<MessageDto>& body){
      return _return(controller->createDtoResponse(Status::CODE_200, body));
    }
    
  };
  
};

#include OATPP_CODEGEN_BEGIN(ApiController)

#endif // __ASYNCCONTROLLER_H__