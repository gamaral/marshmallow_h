add_definitions(-DGAME_LIBRARY)

include_directories(${TINYXML_INCLUDE_DIR})

file(GLOB MARSHMALLOW_GAME_SRCS *.cpp)

add_library(marshmallow_game
	${MARSHMALLOW_GAME_SRCS}
)

target_link_libraries(marshmallow_game
	marshmallow_graphics
	marshmallow_event
	marshmallow_math
	marshmallow_core
	marshmallow_eastl
	${TINYXML_LIBRARIES}
)

