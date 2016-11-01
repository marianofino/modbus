#set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../src)

# If pkg-config is not installed on the system, then the
# CHECK_INSTALL_DIR variable must be set to the install
# location of Check. For example, on Windows, this may
# be: C:/Program Files/check
# set(CHECK_INSTALL_DIR "C:/Program Files/check")

#set (CHECK_INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/ext/lib")

#find_package(Check REQUIRED)
include_directories (${CHECK_INCLUDE_DIRS})

set (TEST_SOURCES
  check_cli_helper.c
)

add_executable (check_cli_helper ${TEST_SOURCES})
add_dependencies(check_cli_helper check)
add_dependencies(check_cli_helper compat)
target_link_libraries (check_cli_helper cli_helper "${CMAKE_CURRENT_BINARY_DIR}/../ext/lib/libcheck.a" "${CMAKE_CURRENT_BINARY_DIR}/../ext/lib/libcompat.a" m rt)
