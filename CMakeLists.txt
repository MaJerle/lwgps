cmake_minimum_required(VERSION 3.0.0)

# Setup project
project(LwGPS VERSION 2.0.1)

# ------------------------
# Configuration used if cmake uses this as
# top-level project CMakeLists.txt file
# This is normally used for development purpose only
if (PROJECT_IS_TOP_LEVEL)
    add_executable(${PROJECT_NAME})

    # Add key executable block
    target_sources(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/dev/VisualStudio/main.c
        ${CMAKE_CURRENT_LIST_DIR}/examples/test_code.c
        )

    target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/dev/VisualStudio
        )

    target_compile_definitions(${PROJECT_NAME} PUBLIC
        WIN32
        _DEBUG
        CONSOLE
        LWGPS_DEV
        )

    # Add subdir with lwgps and link to project
    add_subdirectory("lwgps" lwgps)
    target_link_libraries(${PROJECT_NAME} lwgps)
else()
    message("LWGPS: For library link, use CMakeLists.txt in /lwgps directory")
endif()