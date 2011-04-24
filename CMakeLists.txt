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

option(BUILD_SHARED_LIBS "Build a shared library" OFF)
set(CMAKE_BUILD_TYPE_SHARED_LIBS ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUXX)
	set(CMAKE_CXX_FLAGS "-fno-rtti -fno-exceptions ${CMAKE_CXX_FLAGS}")
endif()

###################################################################### CONTRIB #

set(EASTL_LIBRARIES marshmallow_eastl CACHE INTERNAL "")
set(EASTL_INCLUDE_DIR
    "${PROJECT_SOURCE_DIR}/contrib/eastl/include" CACHE INTERNAL "")

set(TINYXML_LIBRARIES marshmallow_tinyxml CACHE INTERNAL "")
set(TINYXML_INCLUDE_DIR
    "${PROJECT_SOURCE_DIR}/contrib/tinyxml/include" CACHE INTERNAL "")

set(GLPNG_LIBRARIES marshmallow_glpng CACHE INTERNAL "")
set(GLPNG_INCLUDE_DIR
    "${PROJECT_SOURCE_DIR}/contrib/glpng/include" CACHE INTERNAL "")

############################################################### GLOBAL OPTIONS #

set(MARSHMALLOW_NAMESPACE "Marshmallow" CACHE STRING "Marshmallow Namespace")
set(MARSHMALLOW_DEBUG_VERBOSITY "0" CACHE STRING "Verbosity Level")
option(MARSHMALLOW_DEBUG "Marshmallow Debugging" OFF)

set(MARSHMALLOW_VIEWPORT_WIDTH  "640" CACHE STRING "Viewport default width")
set(MARSHMALLOW_VIEWPORT_HEIGHT "480" CACHE STRING "Viewport default height")
set(MARSHMALLOW_VIEWPORT_DEPTH  "24" CACHE STRING "Viewport default depth")
option(MARSHMALLOW_VIEWPORT_FULLSCREEN "Viewport default fullscreen mode" OFF)

option(MARSHMALLOW_VIEWPORT_USE_DUMMY "Use Dummy Viewport (testing)" ON)
option(MARSHMALLOW_VIEWPORT_USE_WGL "Use Wiggle (WGL) Viewport (windows)" OFF)
option(MARSHMALLOW_VIEWPORT_USE_X11_GLX "Use X11-GLX Viewport (unix)" OFF)

option(MARSHMALLOW_CONTRIB_ZLIB "Use bundled ZLIB" ON)
option(MARSHMALLOW_CONTRIB_LIBPNG "Use bundled LibPNG" ON)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	option(MARSHMALLOW_DEBUG "Marshmallow Debugging" ON)
	set(MARSHMALLOW_DEBUG_VERBOSITY "2" CACHE STRING "Verbosity Level")
endif()

####################################################################### CONFIG #

configure_file(
	${PROJECT_SOURCE_DIR}/include/core/config.in.h
	${PROJECT_BINARY_DIR}/include/core/config.h
)
configure_file(
	${PROJECT_SOURCE_DIR}/include/graphics/config.in.h
	${PROJECT_BINARY_DIR}/include/graphics/config.h
)

##################################################################### PLATFORM #

if(UNIX)
	configure_file(
	    ${PROJECT_SOURCE_DIR}/include/core/environment/unix.h
	    ${PROJECT_BINARY_DIR}/include/core/environment.h COPYONLY
	)
elseif(WIN32)
	configure_file(
	    ${PROJECT_SOURCE_DIR}/include/core/environment/win32.h
	    ${PROJECT_BINARY_DIR}/include/core/environment.h COPYONLY
	)
endif()

##################################################################### VIEWPORT #

if(UNIX AND MARSHMALLOW_VIEWPORT_USE_X11_GLX)
	find_package(X11)

	if(APPLE)
		set(OPENGL_FOUND 1)
		include_directories(/usr/X11R6/include)
		link_directories(/usr/X11R6/lib)
		set(OPENGL_LIBRARIES GL X11 GLU glut)
		message(STATUS "X11 GLX on Mac OSX... living on the edge.")
	else()
		find_package(OpenGL)
	endif()

	if(NOT OPENGL_FOUND)
		message(FATAL_ERROR "X11 OpenGL viewport: OpenGL was not found.")
	elseif(NOT X11_FOUND)
		message(FATAL_ERROR "X11 OpenGL viewport: X11 was not found.")
	endif()

	include_directories(${X11_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})
elseif(WIN32 AND MARSHMALLOW_VIEWPORT_USE_WGL)
	find_package(OpenGL)
	include_directories(${OPENGL_INCLUDE_DIR})
endif()

############################################################## SUB DIRECTORIES #

add_subdirectory(contrib)
add_subdirectory(src)
add_subdirectory(demos)

