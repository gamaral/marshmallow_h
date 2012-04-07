add_definitions(-DMARSHMALLOW_GRAPHICS_LIBRARY)

file(GLOB MARSHMALLOW_GRAPHICS_SRCS *.cpp)

set(MARSHMALLOW_GRAPHICS_LIBS marshmallow_core
	                      marshmallow_event
	                      marshmallow_math)

if(MARSHMALLOW_VIEWPORT_OPENGL)

	include_directories(${ZLIB_INCLUDE_DIR})

	set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
	                              opengl/extensions/GLee.c
	                              opengl/factory.cpp
	                              opengl/glpng/glpng.c
	                              opengl/painter.cpp
	                              opengl/texturecoordinatedata.cpp
	                              opengl/texturedata.cpp
	                              opengl/vertexdata.cpp)

	set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS}
	                              ${PNG_LIBRARIES}
	                              ${OPENGL_gl_LIBRARY})

	# GLee requirement
	if(APPLE)
        	FIND_LIBRARY(COREFOUNDATION_LIBRARY CoreFoundation)
		set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS}
		                              ${COREFOUNDATION_LIBRARY})
	endif()

	if(MARSHMALLOW_OPENGL_GLX)
		set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
		                              opengl/viewport-glx.cpp)
		set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS}
		                              ${X11_LIBRARIES} Xxf86vm)
	elseif(MARSHMALLOW_OPENGL_WGL)
		set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
		                              opengl/viewport-wgl.cpp)
	elseif(MARSHMALLOW_OPENGL_SDL)
		include_directories(${SDL_INCLUDE_DIR})
		set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
		                              opengl/viewport-sdl.cpp)
		set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS}
		                              ${SDL_LIBRARY})
	elseif(MARSHMALLOW_OPENGL_QT4)
		include_directories(${QT_INCLUDE_DIR})
		set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
		                              opengl/viewport-qt4.cpp)
		set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS}
		                              ${QT_LIBRARIES})
	else()
		message(WARNING "No OpenGL viewport selected!")
	endif()

	if(MARSHMALLOW_OPENGL_GLES)
		add_definitions(-DMARSHMALLOW_OPENGL_GLES)
	endif()

	add_definitions(-DMARSHMALLOW_VIEWPORT_OPENGL)

elseif(MARSHMALLOW_VIEWPORT_SDL)

	include_directories(${SDL_INCLUDE_DIR})

	set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
		sdl/factory.cpp
		sdl/painter.cpp
		sdl/texturecoordinatedata.cpp
		sdl/texturedata.cpp
		sdl/vertexdata.cpp
		sdl/viewport.cpp)

	set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS}
	                              ${SDL_LIBRARY})

	if(SDLIMAGE_FOUND)
		set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS}
	                                      ${SDLIMAGE_LIBRARY})
		add_definitions(-DWITH_SDL_IMAGE)
	endif()

elseif(MARSHMALLOW_VIEWPORT_DUMMY)
	message(WARNING "You currently only have MARSHMALLOW_VIEWPORT_DUMMY enabled!\n"
	                "Please don't forget to select your TARGET VIEWPORT.")
	set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
	                              dummy/factory.cpp
	                              dummy/painter.cpp
	                              dummy/viewport.cpp)

	if(MARSHMALLOW_DUMMY_FAIL)
		add_definitions(-DWITH_DUMMY_FAIL)
	endif()
endif()

# Used directly by tests and deserializers.
set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
	                      dummy/texturecoordinatedata.cpp
	                      dummy/texturedata.cpp
	                      dummy/vertexdata.cpp)

add_library(marshmallow_graphics ${MARSHMALLOW_GRAPHICS_SRCS})

target_link_libraries(marshmallow_graphics ${MARSHMALLOW_GRAPHICS_LIBS})
