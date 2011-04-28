add_definitions(-DGAME_LIBRARY)

include_directories(${BOX2D_INCLUDE_DIR}
                    ${TINYXML_INCLUDE_DIR})

file(GLOB MARSHMALLOW_GAME_SRCS *.cpp)

add_library(marshmallow_game
	${MARSHMALLOW_GAME_SRCS}
)

target_link_libraries(marshmallow_game
	marshmallow_core
	marshmallow_eastl
	marshmallow_event
	marshmallow_graphics
	marshmallow_math
	${BOX2D_LIBRARIES}
	${TINYXML_LIBRARIES}
)

