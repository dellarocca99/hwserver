cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(sasapi VERSION 1.0.0 LANGUAGES CXX)

## we'll ignore deprecation warnings
add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

add_library(sasapi
  sasapi.h
  utils.h
  utils.cpp
  tito.h
  tito.cpp
  accounting.cpp
  accounting.h
  aft.cpp
  aft.h
  contadores.cpp
  contadores.h
  engconfig.cpp
  engconfig.h
  engineapi.h
  enginecom.h
  enginecom.cpp
  enginedefs.h
  file_log.h
  file_log.c
  machine.cpp
  machine.h
  serial.cpp
  serial.h
  uart_test/Core/Src/enginesvrdefs.h
  uart_test/Core/Src/gendefs.h
  uart_test/Core/Src/sasstructs.h
)

set_target_properties(sasapi PROPERTIES
        CXX_STANDARD 11
        CXX_EXTENSIONS OFF
        CXX_STANDARD_REQUIRED ON
)

target_include_directories(sasapi PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
