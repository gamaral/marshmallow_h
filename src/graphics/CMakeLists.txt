add_definitions(-DGRAPHICS_LIBRARY)

file(GLOB MARSHMALLOW_GRAPHICS_SRCS *.cpp)

if (UNIX AND VIEWPORT_X11_OPENGL)
	include_directories(${X11_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})
	set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
	                              viewport/x11_opengl.cpp)
	set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS}
	                              ${OPENGL_LIBRARIES}
	                              ${X11_LIBRARIES})
elseif (VIEWPORT_SDL)
	include_directories(${SDL_INCLUDE_DIR})
	set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
	                              viewport/sdl.cpp)
	set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS}
	                              ${SDL_LIBRARY})
endif (UNIX AND VIEWPORT_X11_OPENGL)

add_library(marshmallow_graphics STATIC
	${MARSHMALLOW_GRAPHICS_SRCS}
)

target_link_libraries(marshmallow_graphics
	marshmallow_event
	marshmallow_math
	marshmallow_core
	marshmallow_eastl
	${MARSHMALLOW_GRAPHICS_LIBS}
)

