add_definitions(-DGRAPHICS_LIBRARY)

file(GLOB MARSHMALLOW_GRAPHICS_SRCS *.cpp)

if(UNIX AND MARSHMALLOW_VIEWPORT_USE_X11_GLX)
	set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
	                              viewport/x11_glx.cpp
	                              painter/opengl.cpp
	                              textureasset/opengl.cpp)
	set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS}
	                              marshmallow_glpng
	                              ${OPENGL_LIBRARIES}
	                              ${X11_LIBRARIES} Xxf86vm)
elseif(WIN32 AND MARSHMALLOW_VIEWPORT_USE_WGL)
	set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
	                              viewport/wgl.cpp
	                              painter/opengl.cpp
	                              textureasset/opengl.cpp)
	set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS}
	                              marshmallow_glpng
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
	marshmallow_event
	marshmallow_math
	marshmallow_core
	marshmallow_eastl
	${MARSHMALLOW_GRAPHICS_LIBS}
)

