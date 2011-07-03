# EASTL
add_definitions(${EASTL_DEFINITIONS})

add_executable(engine_demo0 "demo0.cpp")

target_link_libraries(engine_demo0 marshmallow_entrypoint
                                   marshmallow_game)

