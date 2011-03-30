project(EASTL)

include_directories(${EASTL_SOURCE_DIR}/include)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB MARSHMALLOW_EASTL_SRCS src/*.cpp)

add_library(marshmallow_eastl STATIC
    ${MARSHMALLOW_EASTL_SRCS}
)

# vim: set sw=4 ts=8 tw=80 noet:
