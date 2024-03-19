# 
# LIB_PREFIX: LWGPS
#
# This file provides set of variables for end user
# and also generates one (or more) libraries, that can be added to the project using target_link_libraries(...)
#
# Before this file is included to the root CMakeLists file (using include() function), user can set some variables:
#
# LWGPS_OPTS_FILE: If defined, it is the path to the user options file. If not defined, one will be generated for you automatically
# LWGPS_COMPILE_OPTIONS: If defined, it provide compiler options for generated library.
# LWGPS_COMPILE_DEFINITIONS: If defined, it provides "-D" definitions to the library build
#

# Custom include directory
set(LWGPS_CUSTOM_INC_DIR ${CMAKE_CURRENT_BINARY_DIR}/lib_inc)

# Library core sources
set(lwgps_core_SRCS 
    ${CMAKE_CURRENT_LIST_DIR}/src/lwgps/lwgps.c
)

# Setup include directories
set(lwgps_include_DIRS
    ${CMAKE_CURRENT_LIST_DIR}/src/include
    ${LWGPS_CUSTOM_INC_DIR}
)

# Register library to the system
add_library(lwgps INTERFACE)
target_sources(lwgps INTERFACE ${lwgps_core_SRCS})
target_include_directories(lwgps INTERFACE ${lwgps_include_DIRS})
target_compile_options(lwgps PRIVATE ${LWGPS_COMPILE_OPTIONS})
target_compile_definitions(lwgps PRIVATE ${LWGPS_COMPILE_DEFINITIONS})

# Create config file if user didn't provide one info himself
if(NOT LWGPS_OPTS_FILE)
    message(STATUS "Using default lwgps_opts.h file")
    set(LWGPS_OPTS_FILE ${CMAKE_CURRENT_LIST_DIR}/src/include/lwgps/lwgps_opts_template.h)
else()
    message(STATUS "Using custom lwgps_opts.h file from ${LWGPS_OPTS_FILE}")
endif()
configure_file(${LWGPS_OPTS_FILE} ${LWGPS_CUSTOM_INC_DIR}/lwgps_opts.h COPYONLY)

