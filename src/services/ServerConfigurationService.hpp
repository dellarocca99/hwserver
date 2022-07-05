#ifndef __SERVERCONFIGURATIONSERVICE_H__
#define __SERVERCONFIGURATIONSERVICE_H__

#include <string>

#include <sasapi.h>

class ServerConfigurationService {
  private:

    /* logging */
    std::string _logSeverity;
    std::string _logFilePath;
    std::string _logFormat;
    std::string _logOutput;


    /* sas */
    int _port;
    char _seid[12];
    std::string _sasCFGFile;
    std::string _seidFile;
    std::shared_ptr<EngineAPI::EngineCom> _engine;


  
  public:
    const std::string CONFIGURATIONS_FILE_NAME = "config.json";
    ServerConfigurationService();

    std::string getLogSeverity();
    std::string getLogFilePath();
    std::string getLogFormat();
    std::string getLogOutput();

    int getPort();
    char * getSEID();
    std::string getSASCFGFile();
    std::string getSEIDFile();
    std::shared_ptr<EngineAPI::EngineCom> getEngine();
};

#endif // __SERVERCONFIGURATIONSERVICE_H__