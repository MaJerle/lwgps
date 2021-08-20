################################################################################
# Lightweight GPS (lwgps) CMake support
################################################################################
# The lwgps library can be configured with a lwgps_opts.h file.
# If such a file is used, the user should have the path containing this file
# in the target include directories in an upper CMakeLists.txt
# 
# Other than that, only two steps are necessary to compile and link against LWGPS:
# 1. Use add_subdirectory to add the lwgps folder
# 2. Link against lwgps with target_link_libraries
################################################################################
cmake_minimum_required(VERSION 3.13)

set(LWGPS_LIB_NAME lwgps)
if(NOT (TARGET ${LWGPS_LIB_NAME}))
    add_library(${LWGPS_LIB_NAME} INTERFACE)
    target_sources(${LWGPS_LIB_NAME} INTERFACE "lwgps/src/lwgps/lwgps.c")
    target_include_directories(${LWGPS_LIB_NAME} INTERFACE "lwgps/src/include")
endif()
