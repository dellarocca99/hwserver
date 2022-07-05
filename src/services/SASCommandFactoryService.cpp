#include "SASCommandFactoryService.hpp"


std::shared_ptr<ISASCommand> SASCommandFactoryService::getCommandInstance(oatpp::Object<CommandDTO> dto)
{
    if (_instanceCreators.find(dto->name->std_str()) == _instanceCreators.end())
        throw new std::runtime_error("El comando solicitado no existe.");

    return SASCommandFactoryService::_instanceCreators[dto->name->std_str()](dto);
}
