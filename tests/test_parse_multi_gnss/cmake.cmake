# CMake include file

# Add more sources
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/test_parse_multi_gnss.c
)

# Options file
set(LWGPS_OPTS_FILE ${CMAKE_CURRENT_LIST_DIR}/lwgps_opts.h)
