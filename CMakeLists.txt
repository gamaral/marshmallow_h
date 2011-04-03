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

# Overrides (DEVELOPMENT)
set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)
if("${CMAKE_BUILD_TYPE}" MATCHES "Debug")
	set(DEBUG ON CACHE INTERNAL "")
	set(DEBUG_VERBOSITY "2" CACHE STRING "Debug verbosity")
endif("${CMAKE_BUILD_TYPE}" MATCHES "Debug")

# Configuration file
configure_file(
	${PROJECT_SOURCE_DIR}/include/config.in.h
	${PROJECT_BINARY_DIR}/include/config.h
)

# Platform specific environment defenitions
if(UNIX)
	set(CMAKE_C_FLAGS_DEBUG "-g -Wall -W -pedantic -Wshadow -Wcast-qual -Wwrite-strings -Wconversion -Wno-variadic-macros -ansi" CACHE STRING "" FORCE)
	set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -W -pedantic -Wshadow -Wcast-qual -Wwrite-strings -Wconversion -Wno-variadic-macros -Wno-long-long -std=c++98" CACHE STRING "" FORCE)
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

add_subdirectory(contrib)
add_subdirectory(src)
add_subdirectory(demos)

