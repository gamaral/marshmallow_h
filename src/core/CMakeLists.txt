add_definitions(-DCORE_LIBRARY)

file(GLOB MARSHMALLOW_CORE_SRCS *.cpp)

if(UNIX)
	set(MARSHMALLOW_CORE_SRCS ${MARSHMALLOW_CORE_SRCS} unix/platform.cpp)
	set(MARSHMALLOW_CORE_LIBS "")
else(WIN32)
	set(MARSHMALLOW_CORE_SRCS ${MARSHMALLOW_CORE_SRCS} win32/platform.cpp)
	set(MARSHMALLOW_CORE_LIBS Winmm)
endif()

add_library(marshmallow_core
	${MARSHMALLOW_CORE_SRCS}
)

target_link_libraries(marshmallow_core marshmallow_eastl
                                       ${MARSHMALLOW_CORE_LIBS}
)

