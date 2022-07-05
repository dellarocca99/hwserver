#ifndef __LOGGINGSERVICE_H__
#define __LOGGINGSERVICE_H__

#include "ServerConfigurationService.hpp"

#include <string>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>

class LoggingService {
  private:
    OATPP_COMPONENT(std::shared_ptr<ServerConfigurationService>, serverConfigurationService);

    std::string buildMessage(const std::string &message);
  public:
    LoggingService();

    void trace(const std::string &message);
    void debug(const std::string &message);
    void info(const std::string &message);
    void warning(const std::string &message);
    void error(const std::string &message);
};

#endif // __LOGGINGSERVICE_H__