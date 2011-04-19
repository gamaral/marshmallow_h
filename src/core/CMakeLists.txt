add_definitions(-DCORE_LIBRARY)

file(GLOB MARSHMALLOW_CORE_SRCS *.cpp)

if(UNIX)
	set(MARSHMALLOW_CORE_SRCS ${MARSHMALLOW_CORE_SRCS} platform/unix.cpp)
	set(MARSHMALLOW_CORE_LIBS "")
else(WIN32)
	set(MARSHMALLOW_CORE_SRCS ${MARSHMALLOW_CORE_SRCS} platform/win32.cpp)
	set(MARSHMALLOW_CORE_LIBS Winmm)
endif()

add_library(marshmallow_core
	${MARSHMALLOW_CORE_SRCS}
)

target_link_libraries(marshmallow_core marshmallow_eastl
                                       ${MARSHMALLOW_CORE_LIBS}
)

