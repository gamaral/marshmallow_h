add_definitions(-DGRAPHICS_LIBRARY)

include_directories(${TINYXML_INCLUDE_DIR})

file(GLOB MARSHMALLOW_GRAPHICS_SRCS *.cpp)

if(OPENGL_FOUND)
	set(MARSHMALLOW_GRAPHICS_OPENGL_SRCS
		opengl/painter.cpp
		opengl/textureasset.cpp)
endif()

if(UNIX AND MARSHMALLOW_VIEWPORT_USE_GLX)
	include_directories(${GLPNG_INCLUDE_DIR})
	set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
	                              ${MARSHMALLOW_GRAPHICS_OPENGL_SRCS}
	                              opengl/viewport-glx.cpp)
	set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS}
	                              ${GLPNG_LIBRARIES}
	                              ${OPENGL_LIBRARIES}
	                              ${X11_LIBRARIES} Xxf86vm)
elseif(WIN32 AND MARSHMALLOW_VIEWPORT_USE_WGL)
	include_directories(${GLPNG_INCLUDE_DIR})
	set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
	                              ${MARSHMALLOW_GRAPHICS_OPENGL_SRCS}
	                              opengl/viewport-wgl.cpp)
	set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS}
	                              ${GLPNG_LIBRARIES}
	                              ${OPENGL_LIBRARIES})
elseif(MARSHMALLOW_VIEWPORT_USE_DUMMY)
	set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
	                              dummy/viewport.cpp
	                              dummy/painter.cpp
	                              dummy/textureasset.cpp)
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

