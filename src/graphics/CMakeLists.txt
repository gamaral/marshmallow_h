add_definitions(-DMARSHMALLOW_GRAPHICS_LIBRARY)

configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

file(GLOB MARSHMALLOW_GRAPHICS_SRCS *.cpp *.h)

set(MARSHMALLOW_GRAPHICS_LIBS marshmallow_core
	                      marshmallow_event
	                      marshmallow_input
	                      marshmallow_math)

if(MARSHMALLOW_VIEWPORT_LOCK_WIDTH)
	add_definitions(-DMARSHMALLOW_VIEWPORT_LOCK_WIDTH)
endif()

if(MARSHMALLOW_VIEWPORT_OPENGL)
	if(MARSHMALLOW_OPENGL_EGL)
		find_package(EGL)
		include_directories(${EGL_INCLUDE_DIR})
	endif()

	if(MARSHMALLOW_OPENGL_ES2)
		find_package(GLES2)
		include_directories(${GLES2_INCLUDE_DIR})
	else()
		find_package(OpenGL)
		include_directories(${OPENGL_INCLUDE_DIR})
	endif()

	if(NOT OPENGL_FOUND)
		message(FATAL_ERROR "OpenGL was not found and *is* very much a "
		    "requirement for the viewport selected.")
	endif()

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
		find_package(X11)

		set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
		                              opengl/viewport-x11.cpp)

		include_directories(${X11_X11_INCLUDE_PATH})
		set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS}
		                              ${X11_X11_LIB})
		add_definitions(-DMARSHMALLOW_OPENGL_X11)

		if(X11_Xinerama_FOUND)
			include_directories(${X11_Xinerama_INCLUDE_PATH})
			set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS}
			                              ${X11_Xinerama_LIB})
			add_definitions(-DMARSHMALLOW_X11_XINERAMA)
		endif()

		if(MARSHMALLOW_INPUT_UNIX_X11)
			add_definitions(-DMARSHMALLOW_INPUT_UNIX_X11)
		endif()

		if(MARSHMALLOW_X11_GRAB_KEYBOARD)
			add_definitions(-DMARSHMALLOW_X11_GRAB_KEYBOARD)
		endif()

		if(MARSHMALLOW_X11_GRAB_POINTER)
			add_definitions(-DMARSHMALLOW_X11_GRAB_POINTER)
		endif()

		if(MARSHMALLOW_X11_OVERRIDE_REDIRECT)
			add_definitions(-DMARSHMALLOW_X11_OVERRIDE_REDIRECT)
		endif()
	elseif(MARSHMALLOW_OPENGL_WGL)
		set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
		                              opengl/viewport-wgl.cpp)
		add_definitions(-DMARSHMALLOW_OPENGL_WGL)

		if(MARSHMALLOW_INPUT_W32_SYSTEM)
			add_definitions(-DMARSHMALLOW_INPUT_W32_SYSTEM)
		endif()
	elseif(MARSHMALLOW_OPENGL_SDL)
		find_package(SDL)
		include_directories(${SDL_INCLUDE_DIR})
		set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
		                              opengl/viewport-sdl.cpp)
		set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS}
		                              ${SDL_LIBRARY})
		add_definitions(-DMARSHMALLOW_OPENGL_SDL)

		if(MARSHMALLOW_INPUT_MISC_SDL)
			add_definitions(-DMARSHMALLOW_INPUT_MISC_SDL)
		endif()
	elseif(MARSHMALLOW_OPENGL_QT4)
		find_package(Qt4 COMPONENTS QtCore QtGui QtOpenGL)
		include(${QT_USE_FILE})
		include_directories(${QT_INCLUDE_DIR})

		set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
		                              opengl/viewport-qt4.cpp)
		set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS}
		                              ${QT_LIBRARIES})
		add_definitions(-DMARSHMALLOW_OPENGL_QT4)

		if(MARSHMALLOW_INPUT_MISC_QT4)
			add_definitions(-DMARSHMALLOW_INPUT_MISC_QT4)
		endif()

	#
	# Broadcom VideoCore (Used in Raspberry Pi)
	#
	elseif(MARSHMALLOW_OPENGL_VC)
		find_package(BCM_HOST)
		include_directories(${BCM_HOST_INCLUDE_DIRS})

		set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
		                              opengl/viewport-vc.cpp)

		set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS}
		                              ${BCM_HOST_LIBRARIES})
		add_definitions(-DMARSHMALLOW_OPENGL_VC)

	#
	# QNX Screen (Used in Playbook)
	#
	elseif(MARSHMALLOW_OPENGL_QNX)
		set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
		                              opengl/viewport-qnx.cpp)
		set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS}
		                              "bps" "screen")
		add_definitions(-DMARSHMALLOW_OPENGL_QNX)

		if(MARSHMALLOW_INPUT_QNX_SCREEN)
			add_definitions(-DMARSHMALLOW_INPUT_QNX_SCREEN)
		endif()

	#
	# Android JNI
	#
	elseif(MARSHMALLOW_OPENGL_JNI)
		set(MARSHMALLOW_GRAPHICS_SRCS ${MARSHMALLOW_GRAPHICS_SRCS}
		                              opengl/viewport-jni.cpp
		                              opengl/jni/android_native_app_glue.c)
		set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS}
		                              "android" "log")
		add_definitions(-DMARSHMALLOW_OPENGL_JNI)

		if(MARSHMALLOW_INPUT_LINUX_JNI)
			add_definitions(-DMARSHMALLOW_INPUT_LINUX_JNI)
		endif()

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

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/config.h
        DESTINATION include/marshmallow_h/graphics
        COMPONENT development)

install(TARGETS marshmallow_graphics
        LIBRARY DESTINATION lib COMPONENT runtime
        ARCHIVE DESTINATION lib COMPONENT development)

