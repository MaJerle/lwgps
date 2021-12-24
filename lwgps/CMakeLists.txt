cmake_minimum_required(VERSION 3.13)

# Debug message
message("LWGPS: Entering lib source CMakeLists.txt")

# Set library name
set(LWGPS_LIB_NAME "lwgps")

# Register library to the system
add_library(${LWGPS_LIB_NAME} INTERFACE)

# Setup generic source files
target_sources(${LWGPS_LIB_NAME} INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/src/lwgps/lwgps.c
    )

# Setup include directories
target_include_directories(${LWGPS_LIB_NAME} INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/src/include
    )
