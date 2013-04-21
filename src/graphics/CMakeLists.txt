###################################################################### OPTIONS #

set(MARSHMALLOW_GRAPHICS_TITLE  "Marshmallow Engine" CACHE STRING "Build Title")
set(MARSHMALLOW_GRAPHICS_WIDTH  "720" CACHE STRING "Native viewport width")
set(MARSHMALLOW_GRAPHICS_HEIGHT "480" CACHE STRING "Native viewport height")
set(MARSHMALLOW_GRAPHICS_DEPTH  "24"  CACHE STRING "Default viewport depth")
set(MARSHMALLOW_GRAPHICS_VSYNC  "1"   CACHE STRING "Default swap interval (0 = OFF)")

option(MARSHMALLOW_GRAPHICS_FULLSCREEN "Viewport default fullscreen mode"      OFF)
option(MARSHMALLOW_GRAPHICS_LOCK_WIDTH "Use width for aspect ratio resolution" OFF)

option(MARSHMALLOW_GRAPHICS_DUMMY  "Use Dummy Graphics Backend (testing)"             ON)
option(MARSHMALLOW_GRAPHICS_OPENGL "Use OpenGL Graphics Backend (glx, wgl, cgl, ...)" OFF)

################################################################################

add_definitions(-DMARSHMALLOW_GRAPHICS_LIBRARY)

set(MARSHMALLOW_GRAPHICS_CONFIG_H "${CMAKE_CURRENT_BINARY_DIR}/config.h")
configure_file(config.h.in ${MARSHMALLOW_GRAPHICS_CONFIG_H})

set(MARSHMALLOW_GRAPHICS_SRCS "camera.cpp"
                              "color.cpp"
                              "interface.cpp"
                              "meshbase.cpp"
                              "quadmesh.cpp"
                              "tilesetbase.cpp"
                              "transform.cpp"
)

set(MARSHMALLOW_GRAPHICS_HDRS ${MARSHMALLOW_GRAPHICS_CONFIG_H})

set(MARSHMALLOW_GRAPHICS_LIBS "marshmallow_core"
                              "marshmallow_math"
                              "marshmallow_graphics_backend"
)

# used directly by tests and deserializers
list(APPEND MARSHMALLOW_GRAPHICS_SRCS "dummy/texturecoordinatedata.cpp"
                                      "dummy/texturedata.cpp"
                                      "dummy/vertexdata.cpp"
)

# tinyxml
include_directories(${TINYXML2_INCLUDE_DIR})
list(APPEND MARSHMALLOW_GRAPHICS_LIBS ${TINYXML2_LIBRARY})

if(MARSHMALLOW_GRAPHICS_LOCK_WIDTH)
	add_definitions(-DMARSHMALLOW_GRAPHICS_LOCK_WIDTH)
else(MARSHMALLOW_GRAPHICS_LOCK_WIDTH)
	add_definitions(-DMARSHMALLOW_GRAPHICS_LOCK_HEIGHT)
endif()

# backend
if(MARSHMALLOW_GRAPHICS_OPENGL)
	add_subdirectory(opengl)
elseif(MARSHMALLOW_GRAPHICS_DUMMY)
	add_subdirectory(dummy)
endif()

add_library(marshmallow_graphics ${MARSHMALLOW_GRAPHICS_SRCS})

target_link_libraries(marshmallow_graphics ${MARSHMALLOW_GRAPHICS_LIBS})

install(FILES ${MARSHMALLOW_GRAPHICS_CONFIG_H}
        DESTINATION "include/marshmallow_h/graphics"
        COMPONENT development)

install(TARGETS marshmallow_graphics
        LIBRARY DESTINATION lib COMPONENT runtime
        ARCHIVE DESTINATION lib COMPONENT development)

