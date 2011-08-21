include(EASTL)

add_executable(tilemap_demo0 "demo0.cpp")

set_target_properties(tilemap_demo0 PROPERTIES
    COMPILE_FLAGS -DDEMO_CWD=\\"${CMAKE_CURRENT_SOURCE_DIR}\\")

target_link_libraries(tilemap_demo0 marshmallow_entrypoint
                                    marshmallow_game)

