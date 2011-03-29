file(GLOB MARSHMALLOW_EVENTS_SRCS *.cpp)

add_library(marshmallow_events STATIC
	${MARSHMALLOW_EVENTS_SRCS}
)

target_link_libraries(marshmallow_events marshmallow_core)

# vim: set sw=4 ts=8 tw=80 noet:
