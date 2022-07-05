#ifndef __SASCOMMANDFACTORYSERVICE_H__
#define __SASCOMMANDFACTORYSERVICE_H__

#include <abstractions/ISASCommandFactoryService.hpp>

#include <services/LoggingService.hpp>

#include <map>
#include <memory>

class SASCommandFactoryService : public ISASCommandFactoryService{
    protected:
        OATPP_COMPONENT(std::shared_ptr<LoggingService>, loggingService);

    public:
        std::shared_ptr<ISASCommand> getCommandInstance (oatpp::Object<CommandDTO> commandDTO);
        
        //crea un map con pares (nombreComando,punteroAInstancia)
        static inline auto _instanceCreators = std::map<std::string, std::shared_ptr<ISASCommand>(*)(oatpp::Object<CommandDTO>)>();

        //asigna una nueva instancia al comando pasado por parametro
        template<class T>
        static inline void registerInstanceCreator(const std::string &commandName) {
            _instanceCreators[commandName] = &createInstance<T>;
        }

        //crea una nueva instancia de comando
        template<class T>
        static inline std::shared_ptr<ISASCommand> createInstance(oatpp::Object<CommandDTO> dto) {
            return std::make_shared<T>(dto);
        }
};


#endif // __SASCOMMANDFACTORYSERVICE_H__