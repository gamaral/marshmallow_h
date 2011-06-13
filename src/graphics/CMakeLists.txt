add_definitions(-DGRAPHICS_LIBRARY)

include_directories(${TINYXML_INCLUDE_DIR})

file(GLOB MARSHMALLOW_GRAPHICS_SRCS *.cpp)

set(MARSHMALLOW_GRAPHICS_LIBS
	marshmallow_core
	marshmallow_eastl
	marshmallow_event
	marshmallow_math)

if(OPENGL_FOUND)
	include_directories(${GLPNG_INCLUDE_DIR})

	set(MARSHMALLOW_GRAPHICS_OPENGL_SRCS
		opengl/factory.cpp
		opengl/meshdata.cpp
		opengl/painter.cpp
		opengl/textureasset.cpp)

	set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS}
	                              ${GLPNG_LIBRARIES}
	                              ${OPENGL_LIBRARIES})
endif()

if(UNIX AND MARSHMALLOW_VIEWPORT_USE_GLX)
	set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
	                              ${MARSHMALLOW_GRAPHICS_OPENGL_SRCS}
	                              opengl/viewport-glx.cpp)
	set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS}
	                              ${X11_LIBRARIES} Xxf86vm)
elseif(WIN32 AND MARSHMALLOW_VIEWPORT_USE_WGL)
	set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
	                              ${MARSHMALLOW_GRAPHICS_OPENGL_SRCS}
	                              opengl/viewport-wgl.cpp)
elseif(MARSHMALLOW_VIEWPORT_USE_DUMMY)
	set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
	                              dummy/factory.cpp
	                              dummy/meshdata.cpp
	                              dummy/viewport.cpp
	                              dummy/painter.cpp
	                              dummy/textureasset.cpp)
endif()

add_library(marshmallow_graphics
	${MARSHMALLOW_GRAPHICS_SRCS}
)

target_link_libraries(marshmallow_graphics ${MARSHMALLOW_GRAPHICS_LIBS})

