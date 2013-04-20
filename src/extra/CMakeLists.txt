add_definitions(-DMARSHMALLOW_EXTRA_LIBRARY)

file(GLOB MARSHMALLOW_EXTRA_SRCS *.cpp)

set(MARSHMALLOW_EXTRA_LIBS
	marshmallow_core
	marshmallow_event
	marshmallow_game
	marshmallow_graphics
	marshmallow_math
)

# TINYXML
include_directories(${TINYXML2_INCLUDE_DIR})
set(MARSHMALLOW_EXTRA_LIBS ${MARSHMALLOW_EXTRA_LIBS} ${TINYXML2_LIBRARY})

add_library(marshmallow_extra
	${MARSHMALLOW_EXTRA_SRCS}
)

target_link_libraries(marshmallow_extra ${MARSHMALLOW_EXTRA_LIBS})

install(TARGETS marshmallow_extra
        LIBRARY DESTINATION lib COMPONENT runtime
        ARCHIVE DESTINATION lib COMPONENT development)

