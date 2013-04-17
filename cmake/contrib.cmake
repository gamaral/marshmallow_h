# Copyright 2011-2013 Marshmallow Engine. All rights reserved.
# Copyright 2011-2013 Guillermo A. Amaral B. (gamaral) <g@maral.me>.
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

# Math
if(UNIX)
	find_library(M_LIBRARY
	    NAMES m
	    PATHS /usr/lib /usr/local/lib
	)

	if(NOT M_LIBRARY)
		message(FATAL_ERROR "No math library found.")
	endif()

	mark_as_advanced(M_LIBRARY)
elseif(WIN32)
	set(M_LIBRARY "")

	if(MSVC)
	    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
	endif()
endif()

# TinyXML
set(TINYXML_LIBRARY marshmallow_tinyxml)
set(TINYXML_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/contrib/tinyxml2/code")
set(TINYXML_FOUND TRUE)

mark_as_advanced(TINYXML_LIBRARY
                 TINYXML_INCLUDE_DIR
                 TINYXML_FOUND
)

# SQLite3
if(MARSHMALLOW_CONTRIB_SQLITE)
	set(SQLITE_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/contrib/sqlite3/code")

	set(SQLITE_LIBRARY   marshmallow_sqlite3)

	set(SQLITE_FOUND TRUE)

	mark_as_advanced(SQLITE_LIBRARY
	                 SQLITE_INCLUDE_DIR
	                 SQLITE_FOUND
	)

	message(STATUS "Building with bundled SQLite3")
else()
	message(STATUS "Building with system SQLite3")
	find_package(SQLite3 REQUIRED)
endif()

# Box2D
if(MARSHMALLOW_WITH_BOX2D)
	set(BOX2D_BASE "${PROJECT_SOURCE_DIR}/contrib/box2d/code/Box2D" CACHE STRING "")

	set(BOX2D_INCLUDE_DIR  ${BOX2D_BASE}        CACHE STRING "")

	set(BOX2D_LIBRARY   marshmallow_box2d CACHE STRING "")

	set(BOX2D_FOUND TRUE)

	mark_as_advanced(BOX2D_BASE
	                 BOX2D_INCLUDE_DIR
	                 BOX2D_LIBRARY
	                 BOX2D_FOUND
	)

	message(STATUS "Building with Box2D")
endif()

# Lua
if(MARSHMALLOW_WITH_LUA)
	if (MARSHMALLOW_CONTRIB_LUA)
		set(LUA_BASE "${PROJECT_SOURCE_DIR}/contrib/lua/code")

		set(LUA_INCLUDE_DIR ${LUA_BASE}/src
		                    ${LUA_BASE}/extra)

		set(LUA_LIBRARY marshmallow_lua)

		set(LUA_FOUND TRUE)

		mark_as_advanced(LUA_BASE
		                 LUA_INCLUDE_DIR
		                 LUA_LIBRARY
		                 LUA_FOUND
		)

		message(STATUS "Builing with bundled Lua")
	else()
		message(STATUS "Building with system Lua")
		find_package(Lua51 REQUIRED)
	endif()
endif()

# zlib
if(MARSHMALLOW_CONTRIB_ZLIB)
	set(ZLIB_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/contrib/zlib/code
	                     ${PROJECT_BINARY_DIR}/contrib/zlib/code)

	set(ZLIB_LIBRARY marshmallow_zlib)

	set(ZLIB_FOUND TRUE)

	mark_as_advanced(ZLIB_INCLUDE_DIR
	                 ZLIB_LIBRARY
	                 ZLIB_FOUND
	)

	message(STATUS "Building with bundled zlib")
else()
	message(STATUS "Building with system zlib")
	find_package(ZLIB REQUIRED)
endif()

# libpng
if(MARSHMALLOW_CONTRIB_LIBPNG)
	if(NOT BUILD_SHARED_LIBS)
		set(PNG_DEFINITIONS -DPNG_STATIC)
	endif()

	set(PNG_PNG_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/contrib/libpng/code
	                        ${PROJECT_BINARY_DIR}/contrib/libpng/code)

	set(PNG_LIBRARY marshmallow_libpng)

	set(PNG_FOUND TRUE)

	mark_as_advanced(PNG_PNG_INCLUDE_DIR
	                 PNG_LIBRARY
	                 PNG_FOUND
	)

	message(STATUS "Building with bundled libpng")
else()
	message(STATUS "Building with system libpng")
	find_package(PNG REQUIRED)
endif()

# libogg
if(MARSHMALLOW_CONTRIB_LIBOGG)
	set(OGG_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/contrib/libogg/code/include
	                    ${PROJECT_BINARY_DIR}/contrib/libogg/code/include)

	set(OGG_LIBRARY marshmallow_libogg)

	set(OGG_FOUND TRUE)

	message(STATUS "Building with bundled Ogg")
else()
	message(STATUS "Building with system Ogg")
	find_package(Ogg REQUIRED)
endif()

# libvorbis
if(MARSHMALLOW_CONTRIB_LIBVORBIS)
	set(VORBIS_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/contrib/libvorbis/code/include)
	set(VORBIS_VORBIS_INCLUDE_DIR ${VORBIS_INCLUDE_DIRS})
	set(VORBIS_FILE_INCLUDE_DIR ${VORBIS_INCLUDE_DIRS})

	set(VORBIS_VORBIS_LIBRARY marshmallow_libvorbis)
	set(VORBIS_FILE_LIBRARY   marshmallow_libvorbisfile)
	set(VORBIS_LIBRARIES      ${VORBIS_VORBIS_LIBRARY}
	                          ${VORBIS_FILE_LIBRARY}
	)

	set(VORBIS_VORBIS_FOUND TRUE)
	set(VORBIS_FILE_FOUND   TRUE)

	mark_as_advanced(VORBIS_LIBRARIES
	                 VORBIS_INCLUDE_DIRS
	                 VORBIS_VORBIS_LIBRARY
	                 VORBIS_VORBIS_INCLUDE_DIR
	                 VORBIS_FILE_LIBRARY
	                 VORBIS_FILE_INCLUDE_DIR
	                 VORBIS_VORBIS_FOUND
	                 VORBIS_FILE_FOUND
	)

	message(STATUS "Building with bundled libvorbis")
else()
	message(STATUS "Building with system libvorbis")
	find_package(Vorbis REQUIRED)
endif()

