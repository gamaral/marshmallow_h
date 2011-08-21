include(EASTL)

add_definitions(-DDEMO_CWD=\\"${CMAKE_CURRENT_SOURCE_DIR}\\")

add_executable(engine_demo0 "demo0.cpp")

target_link_libraries(engine_demo0 marshmallow_entrypoint
                                   marshmallow_game)

