file(GLOB ANGRYMALLOWS_DEMO_SRCS *.cpp)

add_executable(angrymallows_demo ${ANGRYMALLOWS_DEMO_SRCS})

target_link_libraries(angrymallows_demo marshmallow_entrypoint
                                        marshmallow_game)

