file(GLOB ANGRYMALLOWS_DEMO_SRCS *.cpp)

include(EASTL)
include(Box2D)

add_definitions('-DDEMO_CWD="${CMAKE_CURRENT_SOURCE_DIR}"')

add_executable(angrymallows_demo ${ANGRYMALLOWS_DEMO_SRCS})

target_link_libraries(angrymallows_demo marshmallow_entrypoint
                                        marshmallow_game)

