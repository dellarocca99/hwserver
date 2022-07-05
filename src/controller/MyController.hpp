#ifndef __MYCONTROLLER_H__
#define __MYCONTROLLER_H__

#include "dto/DTOs.hpp"
#include "services/LoggingService.hpp"
#include "services/SASCommunicationService.hpp"
#include "services/SASEngineManagerService.hpp"
#include "utils/threads/SASEngineGameTestThread.hpp"

#include <oatpp/web/server/api/ApiController.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>

#include <thread>
#include <chrono>


#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen

/**
 * Sample Api Controller.
 */
using namespace std::chrono_literals;
class MyController : public oatpp::web::server::api::ApiController {
  private:
    OATPP_COMPONENT(std::shared_ptr<LoggingService>, loggingService);
    //OATPP_COMPONENT(std::shared_ptr<SASCommunicationService>, sasCommunicationService);
    OATPP_COMPONENT(std::shared_ptr<SASEngineManagerService>, sasEngineManagerService);
  public:
    /**
     * Constructor with object mapper.
     * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
     */
    MyController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
      : oatpp::web::server::api::ApiController(objectMapper)
    {}

    ENDPOINT("GET", "/", root) {
      auto dto = MyDto::createShared();
      dto->statusCode = 200;
      dto->message = "Hello World! 200";

      loggingService->trace("Le pegaron al endpoint");

      auto response = createDtoResponse(Status::CODE_200, dto);
      
      response->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
      response->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
      response->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
      return response;
    }
    // TODO Insert Your endpoints here !!!
    
    ENDPOINT("GET", "/connect", connect) {

      auto dto = MyDto::createShared();
      dto->statusCode = 200;
      dto->message = "connect executed";
      
      auto response = createDtoResponse(Status::CODE_200, dto);
      
      response->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
      response->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
      response->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
      
      sasEngineManagerService->connect();

      return response;
    }

    ENDPOINT("GET", "/disconnect", disconnect) {

      auto dto = MyDto::createShared();
      dto->statusCode = 200;
      dto->message = "disconnect executed";
      
      auto response = createDtoResponse(Status::CODE_200, dto);
      
      response->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
      response->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
      response->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
      
      sasEngineManagerService->disconnect();

      return response;
    }

    ENDPOINT("GET", "/stop", stop) {
      auto dto = MyDto::createShared();
      dto->statusCode = 200;
      dto->message = "Game Cycle stopped";
            
      auto response = createDtoResponse(Status::CODE_200, dto);
      
      response->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
      response->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
      response->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");

      sasEngineManagerService->stopGameTestThread();

      return response;
    }

    ENDPOINT("GET", "/start", start) {
      auto dto = MyDto::createShared();
      dto->statusCode = 200;
      dto->message = "GameCycle started";
      
      sasEngineManagerService->startGameTestThread();

      auto response = createDtoResponse(Status::CODE_200, dto);
      
      response->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
      response->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
      response->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
      return response;
    }

    ENDPOINT("GET", "/enableDebugger", enableDebugger) {
      auto dto = MyDto::createShared();
      dto->statusCode = 200;
      dto->message = "debugger enabled";
      
      auto engine = sasEngineManagerService->getEngine();

      engine->protocolDebugger(true);

      auto response = createDtoResponse(Status::CODE_200, dto);
      
      response->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
      response->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
      response->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
      return response;
    }

    ENDPOINT("GET", "/disableDebugger", disableDebugger) {
      auto dto = MyDto::createShared();
      dto->statusCode = 200;
      dto->message = "debugger disabled";
      
      auto engine = sasEngineManagerService->getEngine();

      engine->protocolDebugger(false);

      auto response = createDtoResponse(Status::CODE_200, dto);
      
      response->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
      response->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
      response->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
      return response;
    }
};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen

#endif // __MYCONTROLLER_H__