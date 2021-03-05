################################################################################
# Lightweight GPS (lwgps) CMake support
################################################################################
# The lwgps library can be configured with a lwgps_opts.h file.
# If such a file is used, the user should set the LWGPS_CONFIG_PATH path variable where
# the configuration file is located so that is is included properly.
# 
# Other than that, only two steps are necessary to compile and link against LWGPS:
# 1. Use add_subdirectory to add the lwgps folder
# 2. Link against lwgps with target_link_libraries
################################################################################
cmake_minimum_required(VERSION 3.13)

set(LIB_LWGPS_NAME lwgps)

add_library(${LIB_LWGPS_NAME})

add_subdirectory(${LIB_LWGPS_NAME})
add_subdirectory(examples)

# The project CMakeLists can set a LWGPS_CONFIG_PATH path including the lwgps_opts.h file
# and add it.
if(NOT LWGPS_CONFIG_PATH)
    message(STATUS "Lightweight GPS configuration path not set.")
endif()

# Extract the absolute path of the provided configuration path
if(IS_ABSOLUTE ${LWGPS_CONFIG_PATH})
    set(LWGPS_CONFIG_PATH_ABSOLUTE ${LWGPS_CONFIG_PATH})
else()
    get_filename_component(LWGPS_CONFIG_PATH_ABSOLUTE
        ${LWGPS_CONFIG_PATH} REALPATH BASE_DIR ${CMAKE_SOURCE_DIR}
    )
endif()

target_include_directories(${LIB_LWGPS_NAME} PRIVATE
    ${LWGPS_CONFIG_PATH_ABSOLUTE}
)

