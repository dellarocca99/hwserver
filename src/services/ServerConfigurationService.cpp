#include "ServerConfigurationService.hpp"

#include <boost/property_tree/json_parser.hpp>
#include <fstream>

ServerConfigurationService::ServerConfigurationService() {

  boost::property_tree::ptree configTree;
  boost::property_tree::json_parser::read_json(CONFIGURATIONS_FILE_NAME, configTree);

  _logSeverity = configTree.get<std::string>("logging.severity", "trace");
  _logFilePath = configTree.get<std::string>("logging.filePath", "sas_server_%N.log");
  _logFormat = configTree.get<std::string>("logging.format", "[%TimeStamp%]: %Message%");
  _logOutput = configTree.get<std::string>("logging.output", "console");
  _port = configTree.get<int>("sas.port", 0);
  _sasCFGFile = configTree.get<std::string>("sas.configFile", "");
  _seidFile = configTree.get<std::string>("sas.seidFile", "");

  std::ifstream stream(_seidFile);
  stream.read(_seid, 12);
  stream.close();
}

std::string ServerConfigurationService::getLogSeverity() {
  return _logSeverity;
}

std::string ServerConfigurationService::getLogFilePath() {
  return _logFilePath;
}

std::string ServerConfigurationService::getLogFormat() {
  return _logFormat;
}

std::string ServerConfigurationService::getLogOutput() {
  return _logOutput;
}

int ServerConfigurationService::getPort() 
{
  return _port;
}

char* ServerConfigurationService::getSEID() 
{
  return _seid;
}

std::string ServerConfigurationService::getSASCFGFile() 
{
  return _sasCFGFile;
}

std::string ServerConfigurationService::getSEIDFile() 
{
  return _seidFile;
}

std::shared_ptr<EngineAPI::EngineCom> ServerConfigurationService::getEngine() 
{
  return _engine;
}