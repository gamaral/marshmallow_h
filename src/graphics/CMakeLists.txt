add_definitions(-DGRAPHICS_LIBRARY)

file(GLOB MARSHMALLOW_GRAPHICS_SRCS *.cpp)

set(MARSHMALLOW_GRAPHICS_LIBS
	marshmallow_core
	marshmallow_event
	marshmallow_math)

# TINYXML
include_directories(${TINYXML_INCLUDE_DIR})
set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS} ${TINYXML_LIBRARIES})

if(OPENGL_FOUND)
	include_directories(${GLPNG_INCLUDE_DIR})

	set(MARSHMALLOW_GRAPHICS_OPENGL_SRCS
		opengl/extensions/GLee.c
		opengl/factory.cpp
		opengl/painter.cpp
		opengl/texturecoordinatedata.cpp
		opengl/texturedata.cpp
		opengl/vertexdata.cpp)

	set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS}
	                              ${GLPNG_LIBRARIES}
	                              ${OPENGL_LIBRARIES})

	# GLee requirement
	if(APPLE)
        	FIND_LIBRARY(COREFOUNDATION_LIBRARY CoreFoundation)
		set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS}
					      ${COREFOUNDATION_LIBRARY})
	endif()
endif()

# Used directly by tests and deserializers.
set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
	                      dummy/texturecoordinatedata.cpp
	                      dummy/texturedata.cpp
	                      dummy/vertexdata.cpp)

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
elseif(MARSHMALLOW_VIEWPORT_USE_SDL)
	include_directories(${SDL_INCLUDE_DIR})
	set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
	                              ${MARSHMALLOW_GRAPHICS_OPENGL_SRCS}
	                              opengl/viewport-sdl.cpp)
	set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS}
	                              ${SDL_LIBRARY})
elseif(MARSHMALLOW_VIEWPORT_USE_QT4)
	include_directories(${QT_INCLUDE_DIR})
	set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
	                              ${MARSHMALLOW_GRAPHICS_OPENGL_SRCS}
	                              opengl/viewport-qt4.cpp)
	set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS}
	                              ${QT_LIBRARIES})
elseif(MARSHMALLOW_VIEWPORT_USE_DUMMY)
	message(WARNING "You currently only have MARSHMALLOW_VIEWPORT_USE_DUMMY enabled!\n"
	                "Please don't forget to select your TARGET VIEWPORT.")
	set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
	                              dummy/factory.cpp
	                              dummy/painter.cpp
	                              dummy/viewport.cpp)
endif()

add_library(marshmallow_graphics ${MARSHMALLOW_GRAPHICS_SRCS})

target_link_libraries(marshmallow_graphics ${MARSHMALLOW_GRAPHICS_LIBS})
