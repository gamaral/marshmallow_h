# Copyright 2011-2012 Marshmallow Engine. All rights reserved.
# Copyright 2011-2012 Guillermo A. Amaral B. (gamaral) <g@maral.me>.
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

# TinyXML
set(TINYXML_LIBRARY marshmallow_tinyxml)
set(TINYXML_LIBRARIES marshmallow_tinyxml)
set(TINYXML_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/contrib/tinyxml2/code")
mark_as_advanced(TINYXML_LIBRARIES TINYXML_LIBRARY TINYXML_INCLUDE_DIR)

# SQLite3
if(MARSHMALLOW_CONTRIB_SQLITE)
	set(SQLITE_LIBRARIES marshmallow_sqlite3)
	set(SQLITE_LIBRARY marshmallow_sqlite3)
	set(SQLITE_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/contrib/sqlite3/code")
	mark_as_advanced(SQLITE_LIBRARIES SQLITE_LIBRARY SQLITE_INCLUDE_DIR)
	message(STATUS "Building with bundled SQLite3")
else()
	message(STATUS "Building with system SQLite3")
	find_package(SQLite3 REQUIRED)
endif()

# Box2D
if(MARSHMALLOW_WITH_BOX2D)
	set(BOX2D_BASE "${PROJECT_SOURCE_DIR}/contrib/box2d/code/Box2D" CACHE STRING "")
	set(BOX2D_INCLUDE_DIR ${BOX2D_BASE} CACHE STRING "")
	set(BOX2D_LIBRARY marshmallow_box2d CACHE STRING "")

	set(BOX2D_INCLUDE_DIRS ${BOX2D_INCLUDE_DIR} CACHE STRING "")
	set(BOX2D_LIBRARIES ${BOX2D_LIBRARY} CACHE STRING "")

	mark_as_advanced(BOX2D_BASE BOX2D_INCLUDE_DIR BOX2D_LIBRARY BOX2D_INCLUDE_DIRS BOX2D_LIBRARIES)
	message(STATUS "Building with Box2D")
endif()

# Lua
if(MARSHMALLOW_WITH_LUA)
	if (MARSHMALLOW_CONTRIB_LUA)
		set(LUA_BASE "${PROJECT_SOURCE_DIR}/contrib/lua/code")
		set(LUA_INCLUDE_DIR ${LUA_BASE}/src
		                    ${LUA_BASE}/extra)
		set(LUA_LIBRARY marshmallow_lua)
		message(STATUS "Builing with bundled Lua")
	else()
		message(STATUS "Building with system Lua")
		find_package(Lua51 REQUIRED)
	endif()
endif()

# Zlib
if(MARSHMALLOW_CONTRIB_ZLIB)
	set(ZLIB_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/contrib/zlib/code
	                     ${PROJECT_BINARY_DIR}/contrib/zlib/code)
	set(ZLIB_LIBRARY marshmallow_zlib)
	set(ZLIB_LIBRARIES ${ZLIB_LIBRARY})
	message(STATUS "Building with bundled Zlib")
else()
	message(STATUS "Building with system Zlib")
	find_package(ZLIB REQUIRED)
endif()

# Android
if(MARSHMALLOW_ANDROID)
	set(ANDROID_JNI_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/contrib/android-jni/code
	                     ${PROJECT_BINARY_DIR}/contrib/android-jni/code)
	set(ANDROID_JNI_LIBRARY marshmallow_android_jni)
	set(ANDROID_JNI_LIBRARIES ${ANDROID_JNI_LIBRARY})
	message(STATUS "Building with bundled Android JNI glue")
endif()

if(MARSHMALLOW_VIEWPORT_OPENGL)
	# LibPNG
	if(MARSHMALLOW_CONTRIB_LIBPNG)
		set(PNG_PNG_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/contrib/libpng/code
		                        ${PROJECT_BINARY_DIR}/contrib/libpng/code)
		if(BUILD_SHARED_LIBS)
			set(PNG_DEFINITIONS -DPNG_STATIC)
		endif()
		set(PNG_LIBRARY marshmallow_libpng)
		set(PNG_LIBRARIES ${PNG_LIBRARY})
		message(STATUS "Building with bundled LibPNG")
	else()
		message(STATUS "Building with system LibPNG")
		find_package(PNG REQUIRED)
	endif()
	set(PNG_INCLUDE_DIR ${PNG_PNG_INCLUDE_DIR})
endif()

