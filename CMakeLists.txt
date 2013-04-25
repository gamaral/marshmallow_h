cmake_minimum_required(VERSION 2.8)

project(marshmallow_h)

include(CheckCSourceCompiles)
include(CheckCXXSourceCompiles)

set(CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE ON)

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

if(CMAKE_COMPILER_IS_GNUXX)
	set(CMAKE_CXX_FLAGS "-fno-rtti -fno-exceptions ${CMAKE_CXX_FLAGS}")
elseif(MSVC)
	add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
endif()

if(NOT BUILD_SHARED_LIBS)
    if(WIN32)
      set(CMAKE_FIND_LIBRARY_SUFFIXES .lib;.dll)
    else()
      set(CMAKE_FIND_LIBRARY_SUFFIXES .a;.so)
    endif()
endif()

if(UNIX)
	find_package(M REQUIRED)
else()
	set(M_LIBRARY)
endif()

############################################################### GLOBAL OPTIONS #

set(MARSHMALLOW_NAMESPACE "Marshmallow" CACHE STRING "Marshmallow Namespace")

option(MARSHMALLOW_DEBUG "Marshmallow Debugging" OFF)
set(MARSHMALLOW_DEBUG_VERBOSITY "0" CACHE STRING "Verbosity Level")

##################################################################### INCLUDES #

include_directories(${PROJECT_BINARY_DIR}/src
                    ${PROJECT_SOURCE_DIR}/include
)

############################################################## SUB DIRECTORIES #

add_subdirectory(contrib)
add_subdirectory(src)

####################################################################### TARGET #

if(EXISTS ${PROJECT_SOURCE_DIR}/target/CMakeLists.txt)
	add_subdirectory(target)
elseif(EXISTS ${PROJECT_SOURCE_DIR}/demos/CMakeLists.txt)
	add_subdirectory(demos)
else()
	set(MARSHMALLOW_LIBRARY_ONLY ON)
endif()

###################################################################### INSTALL #

# marshmallow headers

install(DIRECTORY include/
        DESTINATION include/marshmallow_h
        COMPONENT development)

# deployment

if(NOT MARSHMALLOW_LIBRARY_ONLY AND UNIX)
	install(FILES resources/unix/run
	        DESTINATION .
	        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
	                    GROUP_READ GROUP_EXECUTE
	                    WORLD_READ WORLD_EXECUTE
	        COMPONENT deployment)
endif()

