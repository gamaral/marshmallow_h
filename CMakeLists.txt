project(MARSHMALLOW)

cmake_minimum_required(VERSION 2.4.0)
if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

include_directories(AFTER SYSTEM ${EASTL_SOURCE_DIR}/include)
include_directories(AFTER SYSTEM ${PROJECT_BINARY_DIR}/include)
include_directories(AFTER SYSTEM ${PROJECT_SOURCE_DIR}/include)

# Global Options
set(MARSHMALLOW_NAMESPACE "Marshmallow" CACHE STRING "Engine Namespace")

# Global Overrides (DEVELOPMENT)
set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_DEBUG "-g -Wall -W -pedantic -Wshadow -Wcast-qual -Wwrite-strings -Wconversion -ansi" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -W -pedantic -Wshadow -Wcast-qual -Wwrite-strings -Wconversion -std=c++98" CACHE STRING "" FORCE)

if("${CMAKE_BUILD_TYPE}" MATCHES "Debug")
	set(DEBUG ON CACHE INTERNAL "")
	set(DEBUG_VERBOSITY "2" CACHE STRING "Debug verbosity")
else("${CMAKE_BUILD_TYPE}" MATCHES "Debug")
	set(DEBUG OFF CACHE INTERNAL "")
endif("${CMAKE_BUILD_TYPE}" MATCHES "Debug")

# Configuration file
configure_file(
    ${PROJECT_SOURCE_DIR}/include/config.in.h
    ${PROJECT_BINARY_DIR}/include/config.h
)

# Platform specific environment defenitions
if(UNIX)
	configure_file(
	    ${PROJECT_SOURCE_DIR}/include/core/unix_environment.h
	    ${PROJECT_BINARY_DIR}/include/core/environment.h COPYONLY
	)
endif(UNIX)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(src)
add_subdirectory(demos)

# vim: set sw=4 ts=8 tw=80 noet:
