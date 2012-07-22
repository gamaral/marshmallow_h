add_definitions(-DMARSHMALLOW_GRAPHICS_LIBRARY)

file(GLOB MARSHMALLOW_GRAPHICS_SRCS *.cpp *.h)

set(MARSHMALLOW_GRAPHICS_LIBS marshmallow_core
	                      marshmallow_event
	                      marshmallow_math)

if(MARSHMALLOW_VIEWPORT_GRAB_INPUT)
	add_definitions(-DMARSHMALLOW_VIEWPORT_GRAB_INPUT)
endif()

if(MARSHMALLOW_VIEWPORT_OPENGL)

	include_directories(${ZLIB_INCLUDE_DIR})
	include_directories(${PNG_PNG_INCLUDE_DIR})

	set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
	                              opengl/extensions.cpp
	                              opengl/factory.cpp
	                              opengl/painter.cpp
	                              opengl/texturecoordinatedata.cpp
	                              opengl/texturedata.cpp
	                              opengl/vertexdata.cpp)

	set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS}
	                              ${PNG_LIBRARIES})

	if(MARSHMALLOW_OPENGL_EGL)
		set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS}
		                              ${EGL_LIBRARY})
		add_definitions(-DMARSHMALLOW_OPENGL_EGL)
	endif()

	if(MARSHMALLOW_OPENGL_ES2)
		set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS}
		                              ${GLES2_LIBRARY})
		add_definitions(-DMARSHMALLOW_OPENGL_ES2)
	else()
		set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS}
		                              ${OPENGL_gl_LIBRARY})
	endif()

	if(MARSHMALLOW_OPENGL_X11)
		include_directories(${X11_INCLUDE_DIR})

		set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
		                              opengl/viewport-x11.cpp)
		set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS}
		                              ${X11_X11_LIB})
		add_definitions(-DMARSHMALLOW_OPENGL_X11)
	elseif(MARSHMALLOW_OPENGL_WGL)
		set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
		                              opengl/viewport-wgl.cpp)
		add_definitions(-DMARSHMALLOW_OPENGL_WGL)
	elseif(MARSHMALLOW_OPENGL_SDL)
		include_directories(${SDL_INCLUDE_DIR})
		set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
		                              opengl/viewport-sdl.cpp)
		set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS}
		                              ${SDL_LIBRARY})
		add_definitions(-DMARSHMALLOW_OPENGL_SDL)
	elseif(MARSHMALLOW_OPENGL_QT4)
		include_directories(${QT_INCLUDE_DIR})
		set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
		                              opengl/viewport-qt4.cpp)
		set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS}
		                              ${QT_LIBRARIES})
		add_definitions(-DMARSHMALLOW_OPENGL_QT4)

	#
	# Broadcom VideoCore (Used in Raspberry Pi)
	#
	elseif(MARSHMALLOW_OPENGL_VC)
		include_directories(${BCM_HOST_INCLUDE_DIRS})

		set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
		                              opengl/viewport-vc.cpp)

		set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS}
		                              ${BCM_HOST_LIBRARIES})
		add_definitions(-DMARSHMALLOW_OPENGL_VC)
	else()
		message(WARNING "No OpenGL viewport selected!")
	endif()

	add_definitions(-DMARSHMALLOW_VIEWPORT_OPENGL)

elseif(MARSHMALLOW_VIEWPORT_GX)

	set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
	                              gx/factory.cpp
	                              gx/painter.cpp
	                              gx/texturecoordinatedata.cpp
	                              gx/texturedata.cpp
	                              gx/vertexdata.cpp
	                              gx/viewport.cpp)

	find_library(BTE_LIBRARY
	    NAMES bte
	    PATHS ${DEVKITPRO}/libogc/lib/wii
	)
	find_library(WIIUSE_LIBRARY
	    NAMES wiiuse
	    PATHS ${DEVKITPRO}/libogc/lib/wii
	)

	set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS}
	                              ${WIIUSE_LIBRARY}
	                              ${BTE_LIBRARY})

	add_definitions(-DMARSHMALLOW_VIEWPORT_GX)

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
