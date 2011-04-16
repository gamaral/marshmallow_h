project(MARSHMALLOW)

cmake_minimum_required(VERSION 2.4.0)
if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

# General
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Includes
include_directories(${PROJECT_BINARY_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include)

# Contribs
include_directories(${PROJECT_SOURCE_DIR}/contrib/eastl/include)

# Global Options
set(MARSHMALLOW_NAMESPACE "Marshmallow" CACHE STRING "Engine Namespace")
set(MARSHMALLOW_BUILD_TYPE "STATIC" CACHE STRING "Build either STATIC or SHARED")

set(VIEWPORT_DEFAULT_WIDTH "800" CACHE STRING "Viewport default width")
set(VIEWPORT_DEFAULT_HEIGHT "600" CACHE STRING "Viewport default height")
set(VIEWPORT_DEFAULT_DEPTH "24" CACHE STRING "Viewport default depth")
set(VIEWPORT_DEFAULT_FULLSCREEN "OFF" CACHE BOOL "Viewport default fullscreen mode")

set(VIEWPORT_X11_GLX "ON" CACHE BOOL "Use X11 GLX Viewport")
set(VIEWPORT_SDL "OFF" CACHE BOOL "Use SDL Viewport")
set(VIEWPORT_DUMMY "ON" CACHE BOOL "Use SDL Dummy Viewport")

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	set(DEBUG ON CACHE INTERNAL "")
	set(DEBUG_VERBOSITY "2" CACHE STRING "Debug Verbosity")
else(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	set(DEBUG OFF CACHE INTERNAL "")
endif(${CMAKE_BUILD_TYPE} STREQUAL "Debug")

# Configuration files
configure_file(
	${PROJECT_SOURCE_DIR}/include/core/config.in.h
	${PROJECT_BINARY_DIR}/include/core/config.h
)
configure_file(
	${PROJECT_SOURCE_DIR}/include/graphics/config.in.h
	${PROJECT_BINARY_DIR}/include/graphics/config.h
)

# Platform specific environment defenitions
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
endif(UNIX)

##################################################################### VIEWPORT #

if(UNIX AND VIEWPORT_X11_GLX)
	find_package(X11)

	if(APPLE)
		set(OPENGL_FOUND 1)
		include_directories(/usr/X11R6/include)
		link_directories(/usr/X11R6/lib)
		set(OPENGL_LIBRARIES GL X11 GLU glut)
		message(STATUS "X11 GLX on Mac OSX... living on the edge.")
	else(APPLE)
		find_package(OpenGL)
	endif(APPLE)

	if(NOT OPENGL_FOUND)
		message(FATAL_ERROR "X11 OpenGL viewport: OpenGL was not found.")
	elseif(NOT X11_FOUND)
		message(FATAL_ERROR "X11 OpenGL viewport: X11 was not found.")
	endif(NOT OPENGL_FOUND)

	include_directories(${X11_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})
elseif(VIEWPORT_SDL)
	find_package(SDL)

	if(NOT SDL_FOUND)
		message(FATAL_ERROR "SDL viewport: SDL was not found.")
	endif(NOT SDL_FOUND)

	include_directories(${SDL_INCLUDE_DIR})
endif(UNIX AND VIEWPORT_X11_GLX)

############################################################## SUB DIRECTORIES #

add_subdirectory(contrib)
add_subdirectory(src)
add_subdirectory(demos)

