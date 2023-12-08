cmake_minimum_required(VERSION 3.22)

# Register library to the system
add_library(lwgps INTERFACE)
target_sources(lwgps INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/lwgps/lwgps.c)
target_include_directories(lwgps INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/include)

# Create config file
if(DEFINED LWGPS_OPTS_DIR AND NOT EXISTS ${LWGPS_OPTS_DIR}/lwgps_opts.h)
    configure_file(${CMAKE_CURRENT_LIST_DIR}/src/include/lwgps/lwgps_opts_template.h ${LWGPS_OPTS_DIR}/lwgps_opts.h COPYONLY)
endif()
