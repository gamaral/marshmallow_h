###################################################################### OPTIONS #

option(MARSHMALLOW_OPENGL_EGL "Use EGL context (x11, jni, bb, vc, ...)"   OFF)
option(MARSHMALLOW_OPENGL_ES2 "Use OpenGL ES 2.0 (x11, jni, bb, vc, ...)" OFF)

option(MARSHMALLOW_OPENGL_BB  "Use OpenGL BlackBerry Screen Backend (bb)" OFF)
option(MARSHMALLOW_OPENGL_JNI "Use OpenGL Android JNI Backend (jni)"      OFF)
option(MARSHMALLOW_OPENGL_QT4 "Use OpenGL Qt4 Backend (qt4)"              OFF)
option(MARSHMALLOW_OPENGL_SDL "Use OpenGL SDL Backend (sdl)"              OFF)
option(MARSHMALLOW_OPENGL_VC  "Use OpenGL VideoCore Backend (broadcom)"   OFF)
option(MARSHMALLOW_OPENGL_WGL "Use OpenGL WGL Backend (windows)"          OFF)
option(MARSHMALLOW_OPENGL_X11 "Use OpenGL X11 & GLX/EGL Backend (unix)"   OFF)

################################################################################

set(MARSHMALLOW_GRAPHICS_OPENGL_SRCS ${MARSHMALLOW_GRAPHICS_OPENGL_SRCS}
	                             extensions.cpp
	                             factory.cpp
	                             painter.cpp
	                             texturecoordinatedata.cpp
	                             texturedata.cpp
	                             vertexdata.cpp)

set(MARSHMALLOW_GRAPHICS_OPENGL_LIBS ${MARSHMALLOW_GRAPHICS_OPENGL_LIBS}
	                             ${PNG_LIBRARIES})

include_directories(${PNG_PNG_INCLUDE_DIR})
include_directories(${ZLIB_INCLUDE_DIR})

if(MARSHMALLOW_OPENGL_EGL)
	find_package(EGL)

	set(MARSHMALLOW_GRAPHICS_OPENGL_LIBS ${MARSHMALLOW_GRAPHICS_OPENGL_LIBS}
	                                     ${EGL_LIBRARY})

	include_directories(${EGL_INCLUDE_DIR})

	add_definitions(-DMARSHMALLOW_OPENGL_EGL)
endif()

if(MARSHMALLOW_OPENGL_ES2)
	find_package(GLES2)

	set(MARSHMALLOW_GRAPHICS_OPENGL_LIBS ${MARSHMALLOW_GRAPHICS_OPENGL_LIBS}
	                                     ${GLES2_LIBRARY})

	include_directories(${GLES2_INCLUDE_DIR})

	add_definitions(-DMARSHMALLOW_OPENGL_ES)
	add_definitions(-DMARSHMALLOW_OPENGL_ES2)
else()
	find_package(OpenGL)

	set(MARSHMALLOW_GRAPHICS_OPENGL_LIBS ${MARSHMALLOW_GRAPHICS_OPENGL_LIBS}
	                                     ${OPENGL_gl_LIBRARY})

	include_directories(${OPENGL_INCLUDE_DIR})
endif()

if(NOT OPENGL_FOUND)
	message(FATAL_ERROR "OpenGL was not found and *is* very much a "
	                    "requirement for the subsystem selected.")
endif()

if(MARSHMALLOW_OPENGL_X11)
	option(MARSHMALLOW_X11_GRAB_KEYBOARD     "Grab keyboard while in fullscreen mode" ON)
	option(MARSHMALLOW_X11_GRAB_POINTER      "Grab pointer while in fullscreen mode"  ON)
	option(MARSHMALLOW_X11_OVERRIDE_REDIRECT "Override redirect in fullscreen mode"   ON)

	set(MARSHMALLOW_GRAPHICS_OPENGL_SRCS ${MARSHMALLOW_GRAPHICS_OPENGL_SRCS}
	                                     backend-x11.cpp)

	find_package(X11)

	set(MARSHMALLOW_GRAPHICS_OPENGL_LIBS ${MARSHMALLOW_GRAPHICS_OPENGL_LIBS}
	                                     ${X11_X11_LIB})

	include_directories(${X11_X11_INCLUDE_PATH})

	add_definitions(-DMARSHMALLOW_OPENGL_X11)

	if(X11_Xinerama_FOUND)
		set(MARSHMALLOW_GRAPHICS_OPENGL_LIBS ${MARSHMALLOW_GRAPHICS_OPENGL_LIBS}
		                                     ${X11_Xinerama_LIB})

		include_directories(${X11_Xinerama_INCLUDE_PATH})
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
	set(MARSHMALLOW_GRAPHICS_OPENGL_SRCS ${MARSHMALLOW_GRAPHICS_OPENGL_SRCS}
	                                     backend-wgl.cpp)


	add_definitions(-DMARSHMALLOW_OPENGL_WGL)

	if(MARSHMALLOW_INPUT_W32_SYSTEM)
		add_definitions(-DMARSHMALLOW_INPUT_W32_SYSTEM)
	endif()
elseif(MARSHMALLOW_OPENGL_SDL)
	set(MARSHMALLOW_GRAPHICS_OPENGL_SRCS ${MARSHMALLOW_GRAPHICS_OPENGL_SRCS}
	                                     backend-sdl.cpp)

	find_package(SDL)

	set(MARSHMALLOW_GRAPHICS_OPENGL_LIBS ${MARSHMALLOW_GRAPHICS_OPENGL_LIBS}
	                                     ${SDL_LIBRARY})

	include_directories(${SDL_INCLUDE_DIR})

	add_definitions(-DMARSHMALLOW_OPENGL_SDL)

	if(MARSHMALLOW_INPUT_MISC_SDL)
		add_definitions(-DMARSHMALLOW_INPUT_MISC_SDL)
	endif()
elseif(MARSHMALLOW_OPENGL_QT4)
	set(MARSHMALLOW_GRAPHICS_OPENGL_SRCS ${MARSHMALLOW_GRAPHICS_OPENGL_SRCS}
	                                     backend-qt4.cpp)

	find_package(Qt4 COMPONENTS QtCore QtGui QtOpenGL)
	include(${QT_USE_FILE})

	set(MARSHMALLOW_GRAPHICS_OPENGL_LIBS ${MARSHMALLOW_GRAPHICS_OPENGL_LIBS}
	                                     ${QT_LIBRARIES})

	include_directories(${QT_INCLUDE_DIR})

	add_definitions(-DMARSHMALLOW_OPENGL_QT4)

	if(MARSHMALLOW_INPUT_MISC_QT4)
		add_definitions(-DMARSHMALLOW_INPUT_MISC_QT4)
	endif()

#
# Broadcom VideoCore (Used in Raspberry Pi)
#
elseif(MARSHMALLOW_OPENGL_VC)
	set(MARSHMALLOW_GRAPHICS_OPENGL_SRCS ${MARSHMALLOW_GRAPHICS_OPENGL_SRCS}
	                                     backend-vc.cpp)

	find_package(BCM_HOST)

	set(MARSHMALLOW_GRAPHICS_OPENGL_LIBS ${MARSHMALLOW_GRAPHICS_OPENGL_LIBS}
	                                     ${BCM_HOST_LIBRARIES})

	include_directories(${BCM_HOST_INCLUDE_DIRS})

	add_definitions(-DMARSHMALLOW_OPENGL_VC)

#
# BlackBerry Screen (QNX)
#
elseif(MARSHMALLOW_OPENGL_BB)
	set(MARSHMALLOW_GRAPHICS_OPENGL_SRCS ${MARSHMALLOW_GRAPHICS_OPENGL_SRCS}
	                                     backend-bb.cpp)
	set(MARSHMALLOW_GRAPHICS_OPENGL_LIBS ${MARSHMALLOW_GRAPHICS_OPENGL_LIBS}
	                                     bps
	                                     screen)

	add_definitions(-DMARSHMALLOW_OPENGL_BB)

	if(MARSHMALLOW_INPUT_BB_SCREEN)
		add_definitions(-DMARSHMALLOW_INPUT_BB_SCREEN)
	endif()

	if(MARSHMALLOW_INPUT_BB_SENSOR)
		add_definitions(-DMARSHMALLOW_INPUT_BB_SENSOR)
	endif()


#
# Android JNI
#
elseif(MARSHMALLOW_OPENGL_JNI)
	set(MARSHMALLOW_GRAPHICS_OPENGL_SRCS ${MARSHMALLOW_GRAPHICS_OPENGL_SRCS}
	                                     backend-jni.cpp)
	set(MARSHMALLOW_GRAPHICS_OPENGL_LIBS ${MARSHMALLOW_GRAPHICS_OPENGL_LIBS}
	                                     ${ANDROID_JNI_LIBRARY}
	                                     marshmallow_entrypoint)

	include_directories(${ANDROID_JNI_INCLUDE_DIR})

	add_definitions(-DMARSHMALLOW_OPENGL_JNI)

	if(MARSHMALLOW_INPUT_LINUX_JNI)
		add_definitions(-DMARSHMALLOW_INPUT_LINUX_JNI)
	endif()

else()
	message(WARNING "No OpenGL subsystem variant selected!")
endif()

add_definitions(-DMARSHMALLOW_GRAPHICS_OPENGL)

add_library(marshmallow_graphics_opengl ${MARSHMALLOW_GRAPHICS_OPENGL_SRCS})

target_link_libraries(marshmallow_graphics_opengl ${MARSHMALLOW_GRAPHICS_OPENGL_LIBS})

