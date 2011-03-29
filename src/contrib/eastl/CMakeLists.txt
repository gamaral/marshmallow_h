project(EASTL)

file(GLOB MARSHMALLOW_EASTL_SRCS src/*.cpp)

add_library(marshmallow_eastl STATIC
    ${MARSHMALLOW_EASTL_SRCS}
)

# vim: set sw=4 ts=8 tw=80 noet:
