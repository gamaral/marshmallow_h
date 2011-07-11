file(GLOB ANGRYMALLOWS_DEMO_SRCS *.cpp)

include(EASTL)
include(Box2D)

add_executable(angrymallows_demo ${ANGRYMALLOWS_DEMO_SRCS})

target_link_libraries(angrymallows_demo marshmallow_entrypoint
                                        marshmallow_game)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/demos/angrymallows)

