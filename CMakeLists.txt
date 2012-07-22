# Copyright 2011 Marshmallow Engine. All rights reserved.
# Copyright 2011 Guillermo A. Amaral B. (gamaral) <g@maral.me>
#
# Redistribution and use in source and binary forms, with or without modification, are
# permitted provided that the following conditions are met:
#
#    1. Redistributions of source code must retain the above copyright notice, this list of
#       conditions and the following disclaimer.
#
#    2. Redistributions in binary form must reproduce the above copyright notice, this list
#       of conditions and the following disclaimer in the documentation and/or other materials
#       provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY MARSHMALLOW ENGINE ``AS IS'' AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
# FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL MARSHMALLOW ENGINE OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# The views and conclusions contained in the software and documentation are those of the
# authors and should not be interpreted as representing official policies, either expressed
# or implied, of Marshmallow Engine.
#

project(MARSHMALLOW)

cmake_minimum_required(VERSION 2.4.3)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)
set(CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE ON)
if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif()

######################################################################## CMAKE #

option(BUILD_SHARED_LIBS "Build a shared library" ON)
option(BUILD_UNIT_TESTS "Build unit tests" OFF)
set(CMAKE_INCLUDE_CURRENT_DIR OFF)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

####################################################################### GLOBAL #

if(BUILD_UNIT_TESTS)
	enable_testing()
endif(BUILD_UNIT_TESTS)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUXX)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
	set(CMAKE_CXX_FLAGS "-fno-rtti -fno-exceptions ${CMAKE_CXX_FLAGS}")
endif()

############################################################### GLOBAL OPTIONS #

set(MARSHMALLOW_NAMESPACE   "Marshmallow"        CACHE STRING "Marshmallow Namespace")
set(MARSHMALLOW_BUILD_TITLE "Marshmallow Engine" CACHE STRING "Build Title")

option(MARSHMALLOW_DEBUG "Marshmallow Debugging" OFF)
set(MARSHMALLOW_DEBUG_VERBOSITY "0" CACHE STRING "Verbosity Level")

set(MARSHMALLOW_VIEWPORT_WIDTH   "720" CACHE STRING "Viewport native width")
set(MARSHMALLOW_VIEWPORT_HEIGHT  "480" CACHE STRING "Viewport native height")
set(MARSHMALLOW_VIEWPORT_DEPTH   "24"  CACHE STRING "Viewport default depth")
set(MARSHMALLOW_VIEWPORT_REFRESH "60"  CACHE STRING "Viewport default refresh rate")

option(MARSHMALLOW_VIEWPORT_FULLSCREEN "Viewport default fullscreen mode"      OFF)
option(MARSHMALLOW_VIEWPORT_GRAB_INPUT "Takeover viewport input devices"        ON)
option(MARSHMALLOW_VIEWPORT_LOCK_WIDTH "Use width for aspect ratio resolution" OFF)
option(MARSHMALLOW_VIEWPORT_VSYNC      "Viewport default vsync mode"            ON)

option(MARSHMALLOW_VIEWPORT_DUMMY  "Use Dummy Viewport (testing)"    ON)
option(MARSHMALLOW_VIEWPORT_GX     "Use GX Viewport (wii)"          OFF)
option(MARSHMALLOW_VIEWPORT_OPENGL "Use OpenGL Viewport (multiple)" OFF)

option(MARSHMALLOW_CONTRIB_LIBPNG "Use bundled LibPNG" ON)
option(MARSHMALLOW_CONTRIB_LUA    "Use bundled LUA"    ON)
option(MARSHMALLOW_CONTRIB_ZLIB   "Use bundled ZLIB"   ON)

option(MARSHMALLOW_WITH_BOX2D "Use Box2D" ON)
option(MARSHMALLOW_WITH_LUA   "Use Lua"   OFF)

########################################################### GLOBAL SUB-OPTIONS #

if(MARSHMALLOW_VIEWPORT_DUMMY)
	option(MARSHMALLOW_DUMMY_FAIL "Make dummy viewport fail initialization" OFF)
endif()

if(MARSHMALLOW_VIEWPORT_OPENGL)
	option(MARSHMALLOW_OPENGL_QT4     "Use OpenGL Qt4 Viewport (qt4)"            OFF)
	option(MARSHMALLOW_OPENGL_SDL     "Use OpenGL SDL Viewport (sdl)"            OFF)
	option(MARSHMALLOW_OPENGL_VC      "Use OpenGL VideoCore Viewport (broadcom)" OFF)
	option(MARSHMALLOW_OPENGL_WGL     "Use OpenGL WGL Viewport (windows)"        OFF)
	option(MARSHMALLOW_OPENGL_X11     "Use OpenGL X11 & GLX/EGL Viewport (unix)" OFF)
	option(MARSHMALLOW_OPENGL_ES2_EGL "Use OpenGL ES 2.0 with EGL (x11/vc)"      OFF)
endif()

####################################################################### CONFIG #

configure_file(
	${PROJECT_SOURCE_DIR}/src/core/config.h.in
	${PROJECT_BINARY_DIR}/include/core/config.h
)
configure_file(
	${PROJECT_SOURCE_DIR}/src/graphics/config.h.in
	${PROJECT_BINARY_DIR}/include/graphics/config.h
)

##################################################################### PLATFORM #

if(UNIX)
	configure_file(
	    ${PROJECT_SOURCE_DIR}/include/core/unix/environment.h
	    ${PROJECT_BINARY_DIR}/include/core/environment.h COPYONLY
	)

	find_library(M_LIBRARY
	    NAMES m
	    PATHS /usr/lib /usr/local/lib
	)
	if(NOT M_LIBRARY)
		message(FATAL_ERROR "No math library found.")
	endif()

	mark_as_advanced(M_LIBRARY)
elseif(WIN32)
	configure_file(
	    ${PROJECT_SOURCE_DIR}/include/core/win32/environment.h
	    ${PROJECT_BINARY_DIR}/include/core/environment.h COPYONLY
	)
	set(M_LIBRARY "")

	if(MSVC)
	    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
	endif()
endif()

##################################################################### VIEWPORT #

if(MARSHMALLOW_VIEWPORT_OPENGL)
	if(MARSHMALLOW_OPENGL_ES2_EGL)
		find_package(GLES2)
		find_package(EGL)
		include_directories(${GLES2_INCLUDE_DIR}
		                    ${EGL_INCLUDE_DIR})
	else()
		find_package(OpenGL)
		include_directories(${OPENGL_INCLUDE_DIR})
	endif()


	if(MARSHMALLOW_OPENGL_X11)
		find_package(X11)
	elseif(MARSHMALLOW_OPENGL_SDL)
		find_package(SDL)
	elseif(MARSHMALLOW_OPENGL_QT4)
		find_package(Qt4 COMPONENTS QtCore QtGui QtOpenGL)
		include(${QT_USE_FILE})
	elseif(MARSHMALLOW_OPENGL_VC)
		find_package(BCM_HOST)
	endif()

	if(NOT OPENGL_FOUND)
		message(FATAL_ERROR "OpenGL was not found and *is* very much a requirement for the viewport selected.")
	endif()
endif()

###################################################################### CONTRIB #

include(cmake/contrib.cmake)

##################################################################### INCLUDES #

include_directories(${PROJECT_BINARY_DIR}/include
                    ${PROJECT_SOURCE_DIR}/include
                    ${TINYXML_INCLUDE_DIR})

############################################################## SUB DIRECTORIES #

add_subdirectory(contrib)
add_subdirectory(src)

####################################################################### TARGET #

if(EXISTS ${PROJECT_SOURCE_DIR}/target/CMakeLists.txt)
	add_subdirectory(target)
else()
	add_subdirectory(demos)
endif()

