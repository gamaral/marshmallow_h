add_definitions(-DENGINE_LIBRARY)

file(GLOB MARSHMALLOW_ENGINE_SRCS *.cpp)

add_library(marshmallow_engine STATIC
	${MARSHMALLOW_ENGINE_SRCS}
)

target_link_libraries(marshmallow_engine marshmallow_event marshmallow_core marshmallow_eastl)

