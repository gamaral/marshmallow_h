add_definitions(-DEXTRA_LIBRARY)

file(GLOB MARSHMALLOW_EXTRA_SRCS *.cpp)

set(MARSHMALLOW_EXTRA_LIBS
	marshmallow_core
	marshmallow_event
	marshmallow_game
	marshmallow_graphics
	marshmallow_math
)

# TINYXML
include_directories(${TINYXML_INCLUDE_DIR})
set(MARSHMALLOW_EXTRA_LIBS ${MARSHMALLOW_EXTRA_LIBS} ${TINYXML_LIBRARIES})

include(EASTL)

add_library(marshmallow_extra
	${MARSHMALLOW_EXTRA_SRCS}
)

target_link_libraries(marshmallow_extra ${MARSHMALLOW_EXTRA_LIBS})
