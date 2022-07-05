#include "headers/TicketOutIdValidationCommand.hpp"

#include <sstream>

TicketOutIdValidationCommand::TicketOutIdValidationCommand(oatpp::Object<CommandDTO> dto) 
{
    auto configDTO = sasEngineManagerService->getConfig();
    auto mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
    auto aux = mapper->writeToString(dto->parameters);
    auto parameters = mapper->readFromString<oatpp::Object<TicketOutIdValidationDTO>>(aux);
    
    if (configDTO->tito->secenh) {
        _valType = SAS_VAL_SE;
    } else if (configDTO->tito->system){
        _valType = SAS_VAL_SYS;

        _type = parameters->type;
        _amnt = parameters->amnt;
    }


}

oatpp::Object<CommandExecutionDTO> TicketOutIdValidationCommand::execute() 
{
    bool ret = false;
    int64_t sysId = 0LL;
    oatpp::List<oatpp::Int64> valId = oatpp::List<oatpp::Int64>::createShared();
    try
    {
        sasEngineManagerService->resetTito();
        auto pTito = sasEngineManagerService->getCurrentTito();


        if (_valType == SAS_VAL_SE) {
            secureEnhValid sev;
            ret = pTito->getValidationID(sev);
            if (ret)
            {
                sysId = 1;  // Fake para Secure Enhanced no se usa
                EngineAPI::valNumber_t sVal;
                EngineAPI::calcValidationSE(sev, sVal.data());

                std::stringstream stream;
                stream << "SE Validated: " << bcdToNum(sVal.data(), 8);
                loggingService->trace(stream.str());
                //LOG_INFO("SE Validated: %llu", bcdToNum(sVal.data(), 8));
                for (uint32_t i = 0; i < sVal.size(); ++i)
                {
                    valId->emplace_back(static_cast<int64_t>(sVal[i]));
                }
            }
        } else if (_valType == SAS_VAL_SYS) {
            systemValid sv;

            ret = pTito->getValidationID(_type, _amnt, sv);

            if (ret)
            {
                sysId = sv.id;
                if (sysId)
                {
                    
                    std::stringstream stream;
                    stream << "Validated. SysId: " << sysId <<" - VID: "<< bcdToNum(sv.number, 8);
                    loggingService->trace(stream.str());
                    for (uint32_t i = 0; i < sizeof(sv.number); ++i)
                    {
                        valId->emplace_back(sv.number[i]);
                    }
                }
                else
                {
                    loggingService->info("Validation refused by host");
                    //LOG_INFO("Validation refused by host");
                }
            }
            else
            {
                loggingService->error("Invalid validation type configured");
                // Error invalid type
                //LOG_ERROR("Invalid validation type configured");
            }
        }
    }
    catch(const std::exception& e)
    {
        //LOG_ERROR("%s->%s", __FUNCTION__, e.what());
        loggingService->error(std::string("Error ticket out id validation ") + e.what());
        sasEngineManagerService->finishTito();
    }

    
    auto result = CommandExecutionDTO::createShared();
    auto resultData = oatpp::UnorderedFields<oatpp::Any>::createShared();

    if (ret)
    {
        resultData["valid"] = valId;
        resultData["sysid"] = oatpp::Int64(sysId);
    }
    
    result->success = ret;
    result->result = resultData;
    return result;
}
