file(GLOB MARSHMALLOW_CORE_SRCS *.cpp)

if(UNIX)
	set(MARSHMALLOW_CORE_SRCS ${MARSHMALLOW_CORE_SRCS} platform/unix.cpp)
endif(UNIX)

add_library(marshmallow_core STATIC
	${MARSHMALLOW_CORE_SRCS}
)

target_link_libraries(marshmallow_core marshmallow_eastl)

