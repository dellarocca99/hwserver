#include "LoggingService.hpp"

#include <thread>

// BOOST LOGGING
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/sources/record_ostream.hpp>

#include <file_log.h>

namespace LOG = boost::log;

LoggingService::LoggingService(){

  std::string logOutput = serverConfigurationService->getLogOutput();
  std::string logFormat = serverConfigurationService->getLogFormat();
  std::string logFilePath = serverConfigurationService->getLogFilePath();
  std::string logSeverity = serverConfigurationService->getLogSeverity();

  if (logOutput == "file") {
		LOG::add_file_log
		(
			LOG::keywords::file_name = logFilePath,
			LOG::keywords::rotation_size = 10 * 1024 * 1024,
			LOG::keywords::time_based_rotation = LOG::sinks::file::rotation_at_time_point(0, 0, 0),
			LOG::keywords::format = logFormat,
			LOG::keywords::open_mode = std::ios_base::app
		);
	}

	if (logOutput == "console") {
		LOG::add_console_log(std::cout, boost::parameter::keyword<LOG::keywords::tag::format>::get() = logFormat);
	}

  LOG::add_common_attributes();
	LOG::trivial::severity_level sl;

	if (logSeverity == "debug")
		sl = LOG::trivial::debug;
	else if (logSeverity == "info")
		sl = LOG::trivial::info;
	else if (logSeverity == "warning")
		sl = LOG::trivial::warning;
	else if (logSeverity == "error")
		sl = LOG::trivial::error;
	else if (logSeverity == "fatal")
		sl = LOG::trivial::fatal;
	else sl = LOG::trivial::trace;

	LOG::core::get()->set_filter
	(
		LOG::trivial::severity >= sl
	);

	// Iniciar el log del sistema
    logger_initFileLogger("mgs.log", 1024 * 1024 * 3, 2);
#if defined(DEBUG) || defined(_DEBUG)
		logger_setLevel(LogLevel_DEBUG);
#else
		logger_setLevel(LogLevel_INFO);
#endif
}

std::string LoggingService::buildMessage(const std::string &message){
  std::stringstream ss;

  ss << "Thread ID: " << std::this_thread::get_id() << " ; Message: " << message;

  return ss.str();
}

void LoggingService::trace(const std::string &message){
  BOOST_LOG_TRIVIAL(trace) << buildMessage(message) << std::endl;
}

void LoggingService::debug(const std::string &message){
  BOOST_LOG_TRIVIAL(debug) << buildMessage(message) << std::endl;
}

void LoggingService::info(const std::string &message){
  BOOST_LOG_TRIVIAL(info) << buildMessage(message) << std::endl;
}

void LoggingService::warning(const std::string &message){
  BOOST_LOG_TRIVIAL(warning) << buildMessage(message) << std::endl;
}

void LoggingService::error(const std::string &message){
  BOOST_LOG_TRIVIAL(error) << buildMessage(message) << std::endl;
}