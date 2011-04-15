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
SET(VIEWPORT_SDL "OFF" CACHE BOOL "Use SDL Viewport")
SET(VIEWPORT_X11_OPENGL "ON" CACHE BOOL "Use X11 OpenGL Viewport")
SET(VIEWPORT_DEFAULT_WIDTH "800" CACHE STRING "Viewport default width")
SET(VIEWPORT_DEFAULT_HEIGHT "600" CACHE STRING "Viewport default height")
SET(VIEWPORT_DEFAULT_DEPTH "24" CACHE STRING "Viewport default depth")
SET(VIEWPORT_DEFAULT_FULLSCREEN "OFF" CACHE BOOL "Viewport default fullscreen mode")

if("${CMAKE_BUILD_TYPE}" MATCHES "Debug")
	set(DEBUG ON CACHE INTERNAL "")
	set(DEBUG_VERBOSITY "2" CACHE STRING "Debug Verbosity")
endif("${CMAKE_BUILD_TYPE}" MATCHES "Debug")

# Configuration file
configure_file(
	${PROJECT_SOURCE_DIR}/include/config.in.h
	${PROJECT_BINARY_DIR}/include/config.h
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

if(UNIX AND VIEWPORT_X11_OPENGL)
	find_package(X11)
	find_package(OpenGL)

	if(APPLE)
		include_directories("/usr/X11R6/include")
	endif(APPLE)

	if(NOT OPENGL_FOUND)
		message(error, "X11 OpenGL viewport: OpenGL was not found.")
	elseif(NOT X11_FOUND)
		message(error, "X11 OpenGL viewport: X11 was not found.")
	endif(NOT OPENGL_FOUND)

	include_directories(${X11_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})
else(VIEWPORT_SDL)
	find_package(SDL)

	if(NOT SDL_FOUND)
		message(FATAL_ERROR "SDL viewport: SDL was not found.")
	endif(NOT SDL_FOUND)

	include_directories(${SDL_INCLUDE_DIR})
endif(UNIX AND VIEWPORT_X11_OPENGL)

############################################################## SUB DIRECTORIES #

add_subdirectory(contrib)
add_subdirectory(src)
add_subdirectory(demos)

