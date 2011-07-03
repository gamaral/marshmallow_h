add_definitions(-DEVENT_LIBRARY)

file(GLOB MARSHMALLOW_EVENT_SRCS *.cpp)
set(MARSHMALLOW_EVENT_LIBS marshmallow_core)

# EASTL
add_definitions(${EASTL_DEFINITIONS})
set(MARSHMALLOW_EVENT_LIBS ${MARSHMALLOW_EVENT_LIBS} ${EASTL_LIBRARIES})

add_library(marshmallow_event
	${MARSHMALLOW_EVENT_SRCS}
)

target_link_libraries(marshmallow_event ${MARSHMALLOW_EVENT_LIBS})

