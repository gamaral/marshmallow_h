set(MARSHMALLOW_TEST_CONFIG_H "${CMAKE_CURRENT_BINARY_DIR}/config.h")
configure_file("config.h.in" ${MARSHMALLOW_TEST_CONFIG_H})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/tests")

add_subdirectory(core)
add_subdirectory(audio)
add_subdirectory(graphics)

