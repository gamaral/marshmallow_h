add_definitions(-DGRAPHICS_LIBRARY)

include_directories(${TINYXML_INCLUDE_DIR})

file(GLOB MARSHMALLOW_GRAPHICS_SRCS *.cpp)

if(UNIX AND MARSHMALLOW_VIEWPORT_USE_X11_GLX)
	include_directories(${GLPNG_INCLUDE_DIR})
	set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
	                              viewport/x11_glx.cpp
	                              painter/opengl.cpp
	                              textureasset/opengl.cpp)
	set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS}
	                              ${GLPNG_LIBRARIES}
	                              ${OPENGL_LIBRARIES}
	                              ${X11_LIBRARIES} Xxf86vm)
elseif(WIN32 AND MARSHMALLOW_VIEWPORT_USE_WGL)
	include_directories(${GLPNG_INCLUDE_DIR})
	set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
	                              viewport/wgl.cpp
	                              painter/opengl.cpp
	                              textureasset/opengl.cpp)
	set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS}
	                              ${GLPNG_LIBRARIES}
	                              ${OPENGL_LIBRARIES})
elseif(MARSHMALLOW_VIEWPORT_USE_DUMMY)
	set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
	                              viewport/dummy.cpp
	                              painter/dummy.cpp
	                              textureasset/dummy.cpp)
endif()

add_library(marshmallow_graphics
	${MARSHMALLOW_GRAPHICS_SRCS}
)

target_link_libraries(marshmallow_graphics
	marshmallow_core
	marshmallow_eastl
	marshmallow_event
	marshmallow_math
	${MARSHMALLOW_GRAPHICS_LIBS}
)

