add_definitions(-DMARSHMALLOW_ENTRYPOINT_LIBRARY)

file(GLOB MARSHMALLOW_ENTRYPOINT_SRCS *.cpp)

if(MARSHMALLOW_WII)
	set(MARSHMALLOW_ENTRYPOINT_SRCS ${MARSHMALLOW_ENTRYPOINT_SRCS} wii/platform.cpp)
elseif(UNIX)
	set(MARSHMALLOW_ENTRYPOINT_SRCS ${MARSHMALLOW_ENTRYPOINT_SRCS} unix/platform.cpp)
	set(MARSHMALLOW_ENTRYPOINT_LIBS marshmallow_event)
elseif(WIN32)
	set(MARSHMALLOW_ENTRYPOINT_SRCS ${MARSHMALLOW_ENTRYPOINT_SRCS} win32/platform.cpp)
endif()

add_library(marshmallow_entrypoint STATIC ${MARSHMALLOW_ENTRYPOINT_SRCS})

target_link_libraries(marshmallow_entrypoint marshmallow_event)

install(TARGETS marshmallow_entrypoint
        ARCHIVE DESTINATION lib COMPONENT development)

