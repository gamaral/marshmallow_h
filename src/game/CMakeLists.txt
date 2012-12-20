###################################################################### OPTIONS #

set(MARSHMALLOW_ENGINE_FRAMERATE "60"  CACHE STRING "Default engine frame rate (FPS)")

################################################################################

add_definitions(-DMARSHMALLOW_GAME_LIBRARY)

configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

file(GLOB MARSHMALLOW_GAME_SRCS *.cpp)

set(MARSHMALLOW_GAME_LIBS
	marshmallow_core
	marshmallow_event
	marshmallow_input
	marshmallow_audio
	marshmallow_graphics
	marshmallow_math)

# BOX2D
if (MARSHMALLOW_WITH_BOX2D)
	file(GLOB MARSHMALLOW_GAME_BOX2D_SRCS box2d/*.cpp)

	set(MARSHMALLOW_GAME_LIBS ${MARSHMALLOW_GAME_LIBS}
	                          ${BOX2D_LIBRARIES})

	include_directories(${BOX2D_INCLUDE_DIR})
endif()

################################################################################

add_library(marshmallow_game
	${MARSHMALLOW_GAME_SRCS}
	${MARSHMALLOW_GAME_BOX2D_SRCS}
)

target_link_libraries(marshmallow_game ${MARSHMALLOW_GAME_LIBS})

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/config.h
        DESTINATION include/marshmallow_h/game
        COMPONENT development)

install(TARGETS marshmallow_game
        LIBRARY DESTINATION lib COMPONENT runtime
        ARCHIVE DESTINATION lib COMPONENT development)

