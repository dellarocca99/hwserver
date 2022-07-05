#ifndef __APPCOMPONENT_H__
#define __APPCOMPONENT_H__

#include <oatpp/web/server/HttpConnectionHandler.hpp>

#include <oatpp/network/tcp/server/ConnectionProvider.hpp>

#include <oatpp/parser/json/mapping/ObjectMapper.hpp>

#include <oatpp/core/macro/component.hpp>

#include <services/SASCommandFactoryService.hpp>
#include <services/ServerConfigurationService.hpp>
#include <services/LoggingService.hpp>
#include <services/SASEngineManagerService.hpp>

#include <model/commands/headers/AFTCashoutCommand.hpp>
#include <model/commands/headers/AFTLockCommand.hpp>
#include <model/commands/headers/AFTTransactionEndCommand.hpp>
#include <model/commands/headers/AFTTransactionStartCommand.hpp>
#include <model/commands/headers/BillinCommand.hpp>
#include <model/commands/headers/ConfigCommand.hpp>
#include <model/commands/headers/CountersCommand.hpp>
#include <model/commands/headers/DateTimeCommand.hpp>
#include <model/commands/headers/EnableCommand.hpp>
#include <model/commands/headers/ExceptionCommand.hpp>
#include <model/commands/headers/GameEndCommand.hpp>
#include <model/commands/headers/GameSelectedCommand.hpp>
#include <model/commands/headers/GameStartCommand.hpp>
#include <model/commands/headers/InitCommand.hpp>
#include <model/commands/headers/NewsCommand.hpp>
#include <model/commands/headers/ReelNStoppedCommand.hpp>
#include <model/commands/headers/SASPacketsDebugCommand.hpp>
#include <model/commands/headers/TicketInCommand.hpp>
#include <model/commands/headers/TicketInResultCommand.hpp>
#include <model/commands/headers/TicketInStatusCommand.hpp>
#include <model/commands/headers/TicketOutIdValidationCommand.hpp>
#include <model/commands/headers/TicketOutValidationRecordCommand.hpp>


/**
 *  Class which creates and holds Application components and registers components in oatpp::base::Environment
 *  Order of components initialization is from top to bottom
 */
class AppComponent {
public:
  
  /**
   *  Create ConnectionProvider component which listens on the port
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)([] {
    return oatpp::network::tcp::server::ConnectionProvider::createShared({"0.0.0.0", 8000, oatpp::network::Address::IP_4});
  }());
  
  /**
   *  Create Router component
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)([] {
    return oatpp::web::server::HttpRouter::createShared();
  }());
  
  /**
   *  Create ConnectionHandler component which uses Router component to route requests
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, serverConnectionHandler)([] {
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router); // get Router component
    return oatpp::web::server::HttpConnectionHandler::createShared(router);
  }());
  
  /**
   *  Create ObjectMapper component to serialize/deserialize DTOs in Contoller's API
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper)([] {
    return oatpp::parser::json::mapping::ObjectMapper::createShared();
  }());

  /**
   *  Create Configurations Tree
   */
  
  OATPP_CREATE_COMPONENT(std::shared_ptr<ServerConfigurationService>, serverConfigurationService)([]{
    return std::make_shared<ServerConfigurationService>();
  }());

  OATPP_CREATE_COMPONENT(std::shared_ptr<SASEngineManagerService>, sasEngineManagerService)([] {
      return std::make_shared<SASEngineManagerService>();
    }());

  OATPP_CREATE_COMPONENT(std::shared_ptr<LoggingService>, loggingService)([]{
    return std::make_shared<LoggingService>();
  }());
  
  
  OATPP_CREATE_COMPONENT(std::shared_ptr<ISASCommandFactoryService>, sasCommandFactoryService)([]{
    SASCommandFactoryService::registerInstanceCreator<AFTCashoutCommand>("AFTHSTCSH");
    SASCommandFactoryService::registerInstanceCreator<AFTLockCommand>("AFTLCKD");
    SASCommandFactoryService::registerInstanceCreator<AFTTransactionEndCommand>("AFTTREND");
    SASCommandFactoryService::registerInstanceCreator<AFTTransactionStartCommand>("AFTTRST");
    SASCommandFactoryService::registerInstanceCreator<BillInCommand>("BILLIN");
    SASCommandFactoryService::registerInstanceCreator<ConfigCommand>("CFG");
    SASCommandFactoryService::registerInstanceCreator<CountersCommand>("CTRS");
    SASCommandFactoryService::registerInstanceCreator<DateTimeCommand>("FYH");
    SASCommandFactoryService::registerInstanceCreator<EnableCommand>("ENA");
    SASCommandFactoryService::registerInstanceCreator<ExceptionCommand>("EX");
    SASCommandFactoryService::registerInstanceCreator<GameEndCommand>("GEND");
    SASCommandFactoryService::registerInstanceCreator<GameSelectedCommand>("SWG");
    SASCommandFactoryService::registerInstanceCreator<GameStartCommand>("GSTART");
    SASCommandFactoryService::registerInstanceCreator<InitCommand>("INIT");
    SASCommandFactoryService::registerInstanceCreator<NewsCommand>("NEWS");
    SASCommandFactoryService::registerInstanceCreator<ReelNStoppedCommand>("REELNS");
    SASCommandFactoryService::registerInstanceCreator<SASPacketsDebugCommand>("ENADBG");
    SASCommandFactoryService::registerInstanceCreator<TicketInCommand>("TKININ");
    SASCommandFactoryService::registerInstanceCreator<TicketInResultCommand>("TKTINTR");
    SASCommandFactoryService::registerInstanceCreator<TicketInStatusCommand>("TKTINST");
    SASCommandFactoryService::registerInstanceCreator<TicketOutIdValidationCommand>("TKTOUTID");
    SASCommandFactoryService::registerInstanceCreator<TicketOutValidationRecordCommand>("TKTOUTVR");

    return std::make_shared<SASCommandFactoryService>();
  }());

/*
  OATPP_CREATE_COMPONENT(std::shared_ptr<SASCommunicationService>, sasCommunicationService)([]{
    return std::make_shared<SASCommunicationService>();
  }());
  OATPP_CREATE_COMPONENT(std::shared_ptr<SASEngineManagerService>, sasEngineManagerService)([]{
    return std::make_shared<SASEngineManagerService>();
  }());
*/
};

#endif // __APPCOMPONENT_H__