cmake_minimum_required (VERSION 2.6)
project (MODBUSCLI)
include(ExternalProject)

set (MODBUSCLI_VERSION_MAJOR 0)
set (MODBUSCLI_VERSION_MINOR 1)
set (MODBUSCLI_VERSION_PATCH 0)

set (MODBUSCLI_EXEC_NAME "modbus_cli_${MODBUSCLI_VERSION_MAJOR}.${MODBUSCLI_VERSION_MINOR}.${MODBUSCLI_VERSION_PATCH}")

configure_file (
    "${PROJECT_SOURCE_DIR}/config.h.in"
    "${PROJECT_SOURCE_DIR}/src/config.h"
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

set (MODBUSCLI_SOURCE_FILES
    "${PROJECT_SOURCE_DIR}/src/main.c"
)

ExternalProject_Add(libmodbus
    #DOWNLOAD_COMMAND ""
    URL "${PROJECT_SOURCE_DIR}/ext/libmodbus-3.0.6.tar.gz"
    CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=<INSTALL_DIR>
    PREFIX "${CMAKE_CURRENT_BINARY_DIR}/ext"
    )


include_directories (${CMAKE_CURRENT_BINARY_DIR}/ext/include)
link_directories (${CMAKE_CURRENT_BINARY_DIR}/ext/lib) 

add_executable (${MODBUSCLI_EXEC_NAME} ${MODBUSCLI_SOURCE_FILES})
target_link_libraries (${MODBUSCLI_EXEC_NAME} modbus)
