add_definitions(-DGAME_LIBRARY)

include_directories(${BOX2D_INCLUDE_DIR}
                    ${TINYXML_INCLUDE_DIR})

file(GLOB MARSHMALLOW_GAME_SRCS *.cpp)

set(MARSHMALLOW_GAME_LIBS
	marshmallow_core
	marshmallow_event
	marshmallow_graphics
	marshmallow_math)

# BOX2D
set(MARSHMALLOW_GAME_LIBS ${MARSHMALLOW_GAME_LIBS} ${BOX2D_LIBRARIES})

# TINYXML
set(MARSHMALLOW_GAME_LIBS ${MARSHMALLOW_GAME_LIBS} ${TINYXML_LIBRARIES})

# EASTL
add_definitions(${EASTL_DEFINITIONS})
set(MARSHMALLOW_GAME_LIBS ${MARSHMALLOW_GAME_LIBS} ${EASTL_LIBRARIES})

add_library(marshmallow_game
	${MARSHMALLOW_GAME_SRCS}
)

target_link_libraries(marshmallow_game ${MARSHMALLOW_GAME_LIBS})
