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
