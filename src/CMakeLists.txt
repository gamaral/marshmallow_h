include_directories(${PROJECT_BINARY_DIR}/src
                    ${PROJECT_SOURCE_DIR}/src)

if (WIN32 AND BUILD_SHARED_LIBS)
    add_definitions(-DMARSHMALLOW_DLL)
endif()

add_subdirectory(core)
add_subdirectory(entrypoint)
add_subdirectory(math)
add_subdirectory(event)
add_subdirectory(graphics)
add_subdirectory(game)
add_subdirectory(tests)

