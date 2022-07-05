cmake_minimum_required(VERSION 3.1)

set(PROJECT_NAME SAS_Server)
set(PROJECT_VERSION 0.1.0) 
set(CMAKE_CXX_STANDARD 17)

option(SAS_BUILD_TESTS "Build application tests" OFF)

project(${PROJECT_NAME} VERSION ${PROJECT_VERSION})

add_library(${PROJECT_NAME}-lib
  src/AppComponent.hpp
  ##src/controller/MyController.cpp
  ##src/controller/MyController.hpp
  ##src/controller/ConfigurationController.cpp
  ##src/controller/ConfigurationController.hpp
  src/abstractions/ISASCommand.hpp
  src/abstractions/ISASCommandFactoryService.hpp
  src/controller/CommandController.cpp
  src/controller/CommandController.hpp
  src/services/ServerConfigurationService.cpp
  src/services/ServerConfigurationService.hpp
  ##src/services/SASMessageService.cpp
  ##src/services/SASMessageService.hpp
  src/services/LoggingService.cpp
  src/services/LoggingService.hpp
  src/services/SASCommandFactoryService.cpp
  src/services/SASCommandFactoryService.hpp
  ##src/services/SASCommunicationService.cpp
  ##src/services/SASCommunicationService.hpp
  src/services/SASEngineManagerService.cpp
  src/services/SASEngineManagerService.hpp
  src/utils/threads/SASEnginePollThread.cpp
  src/utils/threads/SASEnginePollThread.hpp
  src/utils/threads/SASCycledThread.cpp
  src/utils/threads/SASCycledThread.hpp
  src/dto/responses/Response.hpp
  src/dto/requests/Request.hpp
  src/dto/requests/CommandExecutionRequest.hpp
  src/dto/ConfigurationDTO.hpp
  src/dto/CommandDTO.hpp
  src/dto/CommandExecutionDTO.hpp
  src/dto/CommandsDTO.hpp
  src/dto/CountersDTO.hpp
  
  ##commands headers

  src/model/commands/headers/AFTCashoutCommand.hpp
  src/model/commands/headers/AFTLockCommand.hpp
  src/model/commands/headers/AFTTransactionEndCommand.hpp
  src/model/commands/headers/AFTTransactionStartCommand.hpp
  src/model/commands/headers/BillInCommand.hpp
  src/model/commands/headers/ConfigCommand.hpp
  src/model/commands/headers/CountersCommand.hpp
  src/model/commands/headers/DateTimeCommand.hpp
  src/model/commands/headers/EnableCommand.hpp
  src/model/commands/headers/ExceptionCommand.hpp
  src/model/commands/headers/GameEndCommand.hpp
  src/model/commands/headers/GameSelectedCommand.hpp
  src/model/commands/headers/GameStartCommand.hpp
  src/model/commands/headers/InitCommand.hpp
  src/model/commands/headers/NewsCommand.hpp
  src/model/commands/headers/ReelNStoppedCommand.hpp
  src/model/commands/headers/SASPacketsDebugCommand.hpp
  src/model/commands/headers/TicketInCommand.hpp
  src/model/commands/headers/TicketInResultCommand.hpp
  src/model/commands/headers/TicketInStatusCommand.hpp
  src/model/commands/headers/TicketOutIdValidationCommand.hpp
  src/model/commands/headers/TicketOutValidationRecordCommand.hpp

  ##commands sources

  src/model/commands/AFTCashoutCommand.cpp
  src/model/commands/AFTLockCommand.cpp
  src/model/commands/AFTTransactionEndCommand.cpp
  src/model/commands/AFTTransactionStartCommand.cpp
  src/model/commands/BillInCommand.cpp
  src/model/commands/ConfigCommand.cpp
  src/model/commands/CountersCommand.cpp
  src/model/commands/DateTimeCommand.cpp
  src/model/commands/EnableCommand.cpp
  src/model/commands/ExceptionCommand.cpp
  src/model/commands/GameEndCommand.cpp
  src/model/commands/GameSelectedCommand.cpp
  src/model/commands/GameStartCommand.cpp
  src/model/commands/InitCommand.cpp
  src/model/commands/NewsCommand.cpp
  src/model/commands/ReelNStoppedCommand.cpp
  src/model/commands/SASPacketsDebugCommand.cpp
  src/model/commands/TicketInCommand.cpp
  src/model/commands/TicketInResultCommand.cpp
  src/model/commands/TicketInStatusCommand.cpp
  src/model/commands/TicketOutIdValidationCommand.cpp
  src/model/commands/TicketOutValidationRecordCommand.cpp
)

option (OATPP_BUILD_TESTS OFF)

## add subdirectories

add_subdirectory(libs/oatpp)
add_subdirectory(libs/sasapi)

## link libs

target_include_directories(${PROJECT_NAME}-lib
  PUBLIC src
  PUBLIC libs/oatpp
  PUBLIC libs/sasapi
)


link_directories(${PROJECT_NAME}-lib
  PUBLIC libs/oatpp
  PUBLIC libs/sasapi
)

target_link_libraries(${PROJECT_NAME}-lib  
  PUBLIC oatpp
  PUBLIC sasapi
)

## Search libraries

set(Boost_USE_STATIC_LIBS ON) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME ON) 



option(Boost_NO_SYSTEM_PATHS TRUE) 

if (Boost_NO_SYSTEM_PATHS)
  set(BOOST_ROOT "c:/boost/boost_1_75_0")
  set(BOOST_INCLUDE_DIRS "${BOOST_ROOT}/boost")
  set(BOOST_LIBRARY_DIRS "${BOOST_ROOT}/stage/lib")
endif (Boost_NO_SYSTEM_PATHS)

find_package(Boost 1.75.0 REQUIRED) 

if(Boost_FOUND)
  include_directories(${BOOST_INCLUDE_DIRS})
  link_directories(${Boost_LIBRARY_DIRS})
  add_definitions(${Boost_DEFINITIONS})

  include_directories(${Boost_INCLUDE_DIRS})  
  target_link_libraries(${PROJECT_NAME}-lib ${Boost_LIBRARIES})
endif()
## add executables


add_executable(${PROJECT_NAME}-exe
  src/App.cpp
  test/app/MyApiTestClient.hpp)
target_link_libraries(${PROJECT_NAME}-exe ${PROJECT_NAME}-lib)
add_dependencies(${PROJECT_NAME}-exe ${PROJECT_NAME}-lib)



# SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a;.dll")
# target_compile_options(${PROJECT_NAME}-exe PUBLIC /MT)
# target_link_options(${PROJECT_NAME}-exe PUBLIC /INCREMENTAL:NO /NODEFAULTLIB:MSVCRT)
# target_compile_options(${PROJECT_NAME}-lib PUBLIC /MT)
# target_link_options(${PROJECT_NAME}-lib PUBLIC /INCREMENTAL:NO /NODEFAULTLIB:MSVCRT)
# target_compile_options(sasapi PUBLIC /MT)
# target_link_options(sasapi PUBLIC /INCREMENTAL:NO /NODEFAULTLIB:MSVCRT)
# target_compile_options(oatpp PUBLIC /MT)
# target_link_options(oatpp PUBLIC /INCREMENTAL:NO /NODEFAULTLIB:MSVCRT)


set_target_properties(${PROJECT_NAME}-lib ${PROJECT_NAME}-exe PROPERTIES
  CXX_STANDARD 17
  CXX_EXTENSIONS ON
  CXX_STANDARD_REQUIRED ON
)

if (SAS_BUILD_TESTS)
  add_executable(${PROJECT_NAME}-test
    test/tests.cpp
    test/app/TestComponent.hpp
    test/app/MyApiTestClient.hpp
    test/MyControllerTest.cpp
    test/MyControllerTest.hpp
  )

  target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME}-lib)
  add_dependencies(${PROJECT_NAME}-test ${PROJECT_NAME}-lib)

  set_target_properties(${PROJECT_NAME}-test PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS ON
    CXX_STANDARD_REQUIRED ON
  )

  enable_testing()
  add_test(project-tests ${PROJECT_NAME}-test)
endif()

add_custom_command(
    TARGET ${PROJECT_NAME}-exe POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/src/resources/config.json
            $<TARGET_FILE_DIR:${PROJECT_NAME}-exe>/config.json)