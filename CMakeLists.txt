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

if(CMAKE_COMPILER_IS_GNUXX)
	set(CMAKE_CXX_FLAGS "-fno-rtti -fno-exceptions ${CMAKE_CXX_FLAGS}")
endif()

############################################################### GLOBAL OPTIONS #

set(MARSHMALLOW_NAMESPACE "Marshmallow" CACHE STRING "Marshmallow Namespace")

option(MARSHMALLOW_DEBUG "Marshmallow Debugging" OFF)
set(MARSHMALLOW_DEBUG_VERBOSITY "0" CACHE STRING "Verbosity Level")

option(MARSHMALLOW_CONTRIB_LIBPNG "Use bundled LibPNG"  ON)
option(MARSHMALLOW_CONTRIB_LUA    "Use bundled LUA"     ON)
option(MARSHMALLOW_CONTRIB_SQLITE "Use bundled SQLite3" ON)
option(MARSHMALLOW_CONTRIB_ZLIB   "Use bundled ZLIB"    ON)

option(MARSHMALLOW_WITH_BOX2D "Use Box2D" ON)
option(MARSHMALLOW_WITH_LUA   "Use Lua"   OFF)

###################################################################### CONTRIB #

include(cmake/contrib.cmake)

##################################################################### INCLUDES #

include_directories(${PROJECT_BINARY_DIR}/src
                    ${PROJECT_SOURCE_DIR}/include
                    ${TINYXML_INCLUDE_DIR})

####################################################################### TARGET #

if(EXISTS ${PROJECT_SOURCE_DIR}/target/CMakeLists.txt)
	add_subdirectory(target)
elseif(EXISTS ${PROJECT_SOURCE_DIR}/demos/CMakeLists.txt)
	add_subdirectory(demos)
else()
	set(MARSHMALLOW_LIBRARY_ONLY ON)
endif()

############################################################## SUB DIRECTORIES #

add_subdirectory(contrib)
add_subdirectory(src)

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

