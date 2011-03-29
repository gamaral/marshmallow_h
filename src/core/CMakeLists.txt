file(GLOB MARSHMALLOW_CORE_SRCS *.cpp)

if("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
	set(MARSHMALLOW_CORE_SRCS ${MARSHMALLOW_CORE_SRCS} platform/linux.cpp)
endif("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")

add_library(marshmallow_core STATIC
    ${MARSHMALLOW_CORE_SRCS}
)

target_link_libraries(marshmallow_core marshmallow_eastl)

# vim: set sw=4 ts=8 tw=80 noet:
