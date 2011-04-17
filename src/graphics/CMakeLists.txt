add_definitions(-DGRAPHICS_LIBRARY)

file(GLOB MARSHMALLOW_GRAPHICS_SRCS *.cpp)

if(UNIX AND VIEWPORT_X11_GLX)
	set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
	                              viewport/x11_glx.cpp
	                              painter/opengl.cpp)
	set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS}
	                              ${OPENGL_LIBRARIES}
	                              ${X11_LIBRARIES})
elseif(VIEWPORT_SDL)
	set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
	                              viewport/sdl.cpp
	                              painter/opengl.cpp)
	set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS}
	                              ${OPENGL_LIBRARIES}
	                              ${SDL_LIBRARY})
elseif(VIEWPORT_DUMMY)
	set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
	                              viewport/dummy.cpp)
endif(UNIX AND VIEWPORT_X11_GLX)

add_library(marshmallow_graphics ${MARSHMALLOW_BUILD_TYPE}
	${MARSHMALLOW_GRAPHICS_SRCS}
)

target_link_libraries(marshmallow_graphics
	marshmallow_event
	marshmallow_math
	marshmallow_core
	marshmallow_eastl
	${MARSHMALLOW_GRAPHICS_LIBS}
)

