cmake_minimum_required (VERSION 2.6)
project (MODBUSCLI)
include (ExternalProject)

set (MODBUSCLI_VERSION_MAJOR 0)
set (MODBUSCLI_VERSION_MINOR 2)
set (MODBUSCLI_VERSION_PATCH 0)

set (MODBUSCLI_EXEC_NAME "modbus_cli_${MODBUSCLI_VERSION_MAJOR}.${MODBUSCLI_VERSION_MINOR}.${MODBUSCLI_VERSION_PATCH}")

configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_SOURCE_DIR}/src/config.h"
)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

set (CLI_HELPER_SOURCE_FILES
  "${PROJECT_SOURCE_DIR}/src/cli_helper.c"
)
add_library (cli_helper ${CLI_HELPER_SOURCE_FILES})

set (MODBUS_IFC_SOURCE_FILES
  "${PROJECT_SOURCE_DIR}/src/modbus_interface.c"
)
add_library (modbus_interface ${MODBUS_IFC_SOURCE_FILES})
target_link_libraries (modbus_interface modbus)

set (MODBUSCLI_SOURCE_FILES
  "${PROJECT_SOURCE_DIR}/src/main.c"
)

ExternalProject_Add (libmodbus
  URL "${PROJECT_SOURCE_DIR}/ext/libmodbus-3.0.6.tar.gz"
  CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=<INSTALL_DIR>
  PREFIX "${CMAKE_CURRENT_BINARY_DIR}/ext"
)

ExternalProject_Add (check
  PREFIX "${CMAKE_CURRENT_BINARY_DIR}/ext"
  URL "${PROJECT_SOURCE_DIR}/ext/check.tar.gz"
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
)

include_directories (${CMAKE_CURRENT_BINARY_DIR}/ext/include)
link_directories (${CMAKE_CURRENT_BINARY_DIR}/ext/lib)

add_subdirectory(tests)

# Unit tests
enable_testing()
add_test(NAME check_cli_helper COMMAND check_cli_helper)

add_executable (${MODBUSCLI_EXEC_NAME} ${MODBUSCLI_SOURCE_FILES})
target_link_libraries (${MODBUSCLI_EXEC_NAME} cli_helper modbus_interface)
