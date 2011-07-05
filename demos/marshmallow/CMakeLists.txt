file(GLOB MARSHMALLOW_DEMO0_SRCS *.cpp)

# EASTL
add_definitions(${EASTL_DEFINITIONS})

add_executable(marshmallow_demo0 ${MARSHMALLOW_DEMO0_SRCS})

target_link_libraries(marshmallow_demo0 marshmallow_entrypoint
                                        marshmallow_game)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/demos/marshmallow)

