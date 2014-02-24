###################################################################### OPTIONS #

set(MARSHMALLOW_ENGINE_FRAME_RATE_MAX "120" CACHE STRING "Default engine max frame rate (FPS)")

################################################################################

add_definitions(-DMARSHMALLOW_GAME_LIBRARY)

set(MARSHMALLOW_GAME_CONFIG_H "${CMAKE_CURRENT_BINARY_DIR}/config.h")
configure_file("config.h.in" ${MARSHMALLOW_GAME_CONFIG_H})

file(GLOB MARSHMALLOW_GAME_SRCS "*.cpp")
list(REMOVE_ITEM MARSHMALLOW_GAME_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/audiocomponent.cpp")

set(MARSHMALLOW_GAME_HDRS ${MARSHMALLOW_GAME_CONFIG_H})

set(MARSHMALLOW_GAME_LIBS "marshmallow_core"
                          "marshmallow_event"
                          "marshmallow_input"
                          "marshmallow_audio"
                          "marshmallow_graphics"
                          "marshmallow_math"
)

# box2d
if (MARSHMALLOW_WITH_BOX2D)
	include_directories(${BOX2D_INCLUDE_DIR})
	list(APPEND MARSHMALLOW_GAME_LIBS ${BOX2D_LIBRARY})

	file(GLOB MARSHMALLOW_GAME_BOX2D_SRCS "box2d/*.cpp")
	list(APPEND MARSHMALLOW_GAME_SRCS ${MARSHMALLOW_GAME_BOX2D_SRCS})
endif()

add_library(marshmallow_game ${MARSHMALLOW_GAME_SRCS}
                             ${MARSHMALLOW_GAME_HDRS}
)

target_link_libraries(marshmallow_game ${MARSHMALLOW_GAME_LIBS})

install(FILES ${MARSHMALLOW_GAME_CONFIG_H}
        DESTINATION "include/marshmallow_h/game"
        COMPONENT development)

install(TARGETS marshmallow_game
        LIBRARY DESTINATION lib COMPONENT runtime
        ARCHIVE DESTINATION lib COMPONENT development)

