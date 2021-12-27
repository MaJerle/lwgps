cmake_minimum_required(VERSION 3.13)

# Debug message
message("Exiting ${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt")

# Register library to the system
add_library(lwgps INTERFACE)
target_sources(lwgps INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/lwgps/lwgps.c)
target_include_directories(lwgps INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/include)

# Debug message
message("Exiting ${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt")
