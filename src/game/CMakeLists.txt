add_definitions(-DMARSHMALLOW_GAME_LIBRARY)

file(GLOB MARSHMALLOW_GAME_SRCS *.cpp)

set(MARSHMALLOW_GAME_LIBS
	marshmallow_core
	marshmallow_event
	marshmallow_input
	marshmallow_graphics
	marshmallow_math)

# BOX2D
if (MARSHMALLOW_WITH_BOX2D)
	include_directories(${BOX2D_INCLUDE_DIR})
	set(MARSHMALLOW_GAME_LIBS ${MARSHMALLOW_GAME_LIBS} ${BOX2D_LIBRARIES})
	file(GLOB MARSHMALLOW_GAME_BOX2D_SRCS box2d/*.cpp)
endif()

add_library(marshmallow_game
	${MARSHMALLOW_GAME_SRCS}
	${MARSHMALLOW_GAME_BOX2D_SRCS}
)

target_link_libraries(marshmallow_game ${MARSHMALLOW_GAME_LIBS})

install(TARGETS marshmallow_game
        LIBRARY DESTINATION lib COMPONENT runtime
        ARCHIVE DESTINATION lib COMPONENT development)

