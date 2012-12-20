message(WARNING "You currently only have MARSHMALLOW_VIEWPORT_DUMMY enabled!\n"
                "Please don't forget to select your TARGET VIEWPORT.")

if(MARSHMALLOW_GRAPHICS_DUMMY)
	option(MARSHMALLOW_DUMMY_FAIL "Make dummy viewport fail initialization" OFF)
endif()

set(MARSHMALLOW_GRAPHICS_DUMMY_SRCS backend.cpp
                                    factory.cpp
                                    painter.cpp
                                    texturecoordinatedata.cpp
                                    texturedata.cpp
                                    vertexdata.cpp)

add_library(marshmallow_graphics_dummy ${MARSHMALLOW_GRAPHICS_DUMMY_SRCS})

