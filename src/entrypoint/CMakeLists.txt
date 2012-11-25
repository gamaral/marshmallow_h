add_definitions(-DMARSHMALLOW_ENTRYPOINT_LIBRARY)

if(MARSHMALLOW_ANDROID)
	set(MARSHMALLOW_ENTRYPOINT_SRCS ${MARSHMALLOW_ENTRYPOINT_SRCS} "android.cpp")
elseif(MARSHMALLOW_WII)
	set(MARSHMALLOW_ENTRYPOINT_SRCS ${MARSHMALLOW_ENTRYPOINT_SRCS} "wii.cpp")
elseif(UNIX)
	set(MARSHMALLOW_ENTRYPOINT_SRCS ${MARSHMALLOW_ENTRYPOINT_SRCS} "unix.cpp")
	set(MARSHMALLOW_ENTRYPOINT_LIBS marshmallow_event)
elseif(WIN32)
	set(MARSHMALLOW_ENTRYPOINT_SRCS ${MARSHMALLOW_ENTRYPOINT_SRCS} "win32.cpp")
endif()

add_library(marshmallow_entrypoint STATIC ${MARSHMALLOW_ENTRYPOINT_SRCS})

target_link_libraries(marshmallow_entrypoint marshmallow_event)

install(TARGETS marshmallow_entrypoint
        ARCHIVE DESTINATION lib COMPONENT development)

