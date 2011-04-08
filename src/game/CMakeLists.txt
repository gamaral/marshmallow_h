add_definitions(-DGAME_LIBRARY)

file(GLOB MARSHMALLOW_GAME_SRCS *.cpp)

add_library(marshmallow_game STATIC
	${MARSHMALLOW_GAME_SRCS}
)

target_link_libraries(marshmallow_game marshmallow_event marshmallow_core marshmallow_eastl)

