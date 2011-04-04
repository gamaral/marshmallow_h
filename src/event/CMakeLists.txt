add_definitions(-DEVENT_LIBRARY)

file(GLOB MARSHMALLOW_EVENT_SRCS *.cpp)

add_library(marshmallow_event STATIC
	${MARSHMALLOW_EVENT_SRCS}
)

target_link_libraries(marshmallow_event marshmallow_core)

