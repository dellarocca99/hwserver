#ifndef __CONFIGURATIONCONTROLLER_H__
#define __CONFIGURATIONCONTROLLER_H__

#include <dto/ConfigurationDTO.hpp>
#include <dto/DTOs.hpp>

#include <services/LoggingService.hpp>
#include <services/SASEngineManagerService.hpp>

#include <oatpp/web/server/api/ApiController.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen

class ConfigurationController : public oatpp::web::server::api::ApiController{
    private:
        OATPP_COMPONENT(std::shared_ptr<LoggingService>, loggingService);
        OATPP_COMPONENT(std::shared_ptr<SASEngineManagerService>, sasEngineManagerService);

    public: 
        /**
         * Constructor with object mapper.
         * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
         */
        ConfigurationController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
        : oatpp::web::server::api::ApiController(objectMapper)
        {}


        ENDPOINT("GET", "configuration/sas", getSASConfiguration) {
            auto config = sasEngineManagerService->getSASConfiguration();

            return createDtoResponse(Status::CODE_200, config);
        }

        ENDPOINT("POST", "configuration/sas", setSASConfiguration, BODY_DTO(Object<SASConfigDTO>, sasConfigDTO)) {
                
            sasEngineManagerService->setSASConfiguration(sasConfigDTO);

            return createDtoResponse(Status::CODE_200, nullptr);
        }
};

#endif // __CONFIGURATIONCONTROLLER_H__