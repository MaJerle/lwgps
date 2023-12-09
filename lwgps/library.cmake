# 
# This file provides set of variables for end user
# and also generates one (or more) libraries, that can be added to the project using target_link_libraries(...)
#
# Before this file is included to the root CMakeLists file (using include() function), user can set some variables:
#
# LWGPS_OPTS_DIR: If defined, it should set the folder path where options file shall be generated.
# LWGPS_COMPILE_OPTIONS: If defined, it provide compiler options for generated library.
# LWGPS_COMPILE_DEFINITIONS: If defined, it provides "-D" definitions to the library build
#

# Library core sources
set(lwgps_core_SRCS 
    ${CMAKE_CURRENT_LIST_DIR}/src/lwgps/lwgps.c
)

# Setup include directories
set(lwgps_include_DIRS
    ${CMAKE_CURRENT_LIST_DIR}/src/include
)

# Register library to the system
add_library(lwgps INTERFACE)
target_sources(lwgps INTERFACE ${lwgps_core_SRCS})
target_include_directories(lwgps INTERFACE ${lwgps_include_DIRS})
target_compile_options(lwgps PRIVATE ${LWGPS_COMPILE_OPTIONS})
target_compile_definitions(lwgps PRIVATE ${LWGPS_COMPILE_DEFINITIONS})

# Create config file
if(DEFINED LWGPS_OPTS_DIR AND NOT EXISTS ${LWGPS_OPTS_DIR}/lwgps_opts.h)
    configure_file(${CMAKE_CURRENT_LIST_DIR}/src/include/lwgps/lwgps_opts_template.h ${LWGPS_OPTS_DIR}/lwgps_opts.h COPYONLY)
endif()
