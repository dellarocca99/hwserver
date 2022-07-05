#ifndef __COMMANDCONTROLLER_H__
#define __COMMANDCONTROLLER_H__

#include <oatpp/web/server/api/ApiController.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>

#include <services/LoggingService.hpp>
#include <abstractions/ISASCommandFactoryService.hpp>
//#include <services/SASCommandFactoryService.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>

#include <dto/responses/Response.hpp>
#include <dto/CommandDTO.hpp>
#include <dto/CommandExecutionDTO.hpp>

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen

class CommandController : public oatpp::web::server::api::ApiController  {
    private:
        OATPP_COMPONENT(std::shared_ptr<LoggingService>, loggingService);
        OATPP_COMPONENT(std::shared_ptr<ISASCommandFactoryService>, sasCommandFactoryService);
        //OATPP_COMPONENT(std::shared_ptr<SASCommunicationService>, sasCommunicationService);
        //OATPP_COMPONENT(std::shared_ptr<SASEngineManagerService>, sasEngineManagerService);
    public:
        /**
         * Constructor with object mapper.
         * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
         */
        CommandController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
        : oatpp::web::server::api::ApiController(objectMapper)
        {}

        ENDPOINT("POST", "/command", command, BODY_DTO(Object<CommandDTO>, commandDTO)) {
            auto dto = ResponseOk::createShared();
            std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> response;

            loggingService->info("Recibiendo el commando " + commandDTO->name->std_str());

            auto mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
            auto str = mapper->writeToString(commandDTO);

            loggingService->debug("Comando: \n" + str->std_str());

            try {
                auto command = sasCommandFactoryService->getCommandInstance(commandDTO);
                loggingService->trace("Command instanciated successfully");
                auto result = command->execute();
                loggingService->debug("Resultado:\n" + mapper->writeToString(result)->std_str());
                response = createDtoResponse(Status::CODE_200, result);
            } catch (const std::exception &ex){
                response = createDtoResponse(Status::CODE_500, oatpp::String(ex.what()));
                loggingService->error("Error: " +  oatpp::String(ex.what())->std_str());
            }

            return response;
        }

};


#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen
#endif // __COMMANDCONTROLLER_H__