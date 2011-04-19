project(MARSHMALLOW)

cmake_minimum_required(VERSION 2.4.0)
if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif()

# General
option(BUILD_SHARED_LIBS "Build a shared library" OFF)
set(CMAKE_BUILD_TYPE_SHARED_LIBS ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

# Includes
include_directories(${PROJECT_BINARY_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include)

# Contribs
include_directories(${PROJECT_SOURCE_DIR}/contrib/eastl/include)
include_directories(${PROJECT_SOURCE_DIR}/contrib/glpng/include)

# Global Options
set(MARSHMALLOW_NAMESPACE "Marshmallow" CACHE STRING "Marshmallow Namespace")
set(MARSHMALLOW_DEBUG_VERBOSITY "0" CACHE STRING "Verbosity Level")
option(MARSHMALLOW_DEBUG "Marshmallow Debugging" OFF)

set(MARSHMALLOW_VIEWPORT_WIDTH  "640" CACHE STRING "Viewport default width")
set(MARSHMALLOW_VIEWPORT_HEIGHT "480" CACHE STRING "Viewport default height")
set(MARSHMALLOW_VIEWPORT_DEPTH  "24" CACHE STRING "Viewport default depth")
option(MARSHMALLOW_VIEWPORT_FULLSCREEN "Viewport default fullscreen mode" OFF)

option(MARSHMALLOW_VIEWPORT_X11_GLX "Use X11-GLX Viewport (linux)" ON)
option(MARSHMALLOW_VIEWPORT_DUMMY "Use Dummy Viewport (testing)" ON)

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

if(UNIX AND MARSHMALLOW_VIEWPORT_X11_GLX)
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
endif()

################################################################# DEPENDENCIES #

if (OPENGL_FOUND)
	if (MARSHMALLOW_CONTRIB_ZLIB)
		set(ZLIB_FOUND TRUE)
		set(ZLIB_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/contrib/zlib/
		                     ${PROJECT_BINARY_DIR}/contrib/zlib/)
		set(ZLIB_LIBRARIES marshmallow_zlib)
	else()
		find_package(ZLIB)
	endif()

	if(MARSHMALLOW_CONTRIB_LIBPNG)
		set(PNG_FOUND TRUE)
		set(PNG_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/contrib/libpng/
		                    ${PROJECT_BINARY_DIR}/contrib/libpng/)
		set(PNG_DEFINITIONS -DPNG_STATIC)
		set(PNG_LIBRARIES marshmallow_libpng)
		set(PNG_LIBRARY marshmallow_libpng)
	else()
		find_package(PNG)
	endif()
	add_definitions(${PNG_DEFINITIONS})
endif()

############################################################## SUB DIRECTORIES #

add_subdirectory(contrib)
add_subdirectory(src)
add_subdirectory(demos)

