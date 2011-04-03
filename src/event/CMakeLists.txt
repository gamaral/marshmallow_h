add_definitions(-DEVENT_LIBRARY)

file(GLOB MARSHMALLOW_EVENTS_SRCS *.cpp)

add_library(marshmallow_events STATIC
	${MARSHMALLOW_EVENTS_SRCS}
)

target_link_libraries(marshmallow_events marshmallow_core)

