cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(sasapi VERSION 1.0.0 LANGUAGES CXX)

## we'll ignore deprecation warnings
add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

add_library(sasapi
  utils.cpp
  tito.cpp
  accounting.cpp
  aft.cpp
  contadores.cpp
  engconfig.cpp
  enginecom.cpp
  file_log.c
  machine.cpp
  serial.cpp
)

set_target_properties(sasapi PROPERTIES
        CXX_STANDARD 11
        CXX_EXTENSIONS OFF
        CXX_STANDARD_REQUIRED ON
)
