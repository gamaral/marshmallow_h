file(GLOB ANGRYMALLOWS_DEMO_SRCS *.cpp)

include(EASTL)
include(Box2D)

add_executable(angrymallows_demo ${ANGRYMALLOWS_DEMO_SRCS})

set_target_properties(angrymallows_demo PROPERTIES
    COMPILE_FLAGS -DDEMO_CWD=\\"${CMAKE_CURRENT_SOURCE_DIR}\\")

target_link_libraries(angrymallows_demo marshmallow_entrypoint
                                        marshmallow_game)

