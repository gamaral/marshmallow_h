# Copyright 2011 Marshmallow Engine. All rights reserved.
# Copyright 2011 Guillermo A. Amaral B. (gamaral) <g@maral.me>.
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
set(TINYXML_LIBRARIES marshmallow_tinyxml)
set(TINYXML_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tinyxml/include")
add_subdirectory(tinyxml)
set(TINYXML_LIBRARIES ${TINYXML_LIBRARIES} PARENT_SCOPE)
set(TINYXML_INCLUDE_DIR ${TINYXML_INCLUDE_DIR} PARENT_SCOPE)

# Box2D
if (MARSHMALLOW_WITH_BOX2D)
	add_subdirectory(box2d)
endif()

# LIBPNG
if(MARSHMALLOW_CONTRIB_LIBPNG)
	add_subdirectory(libpng)
endif()

# Lua
if (MARSHMALLOW_CONTRIB_LUA)
	add_subdirectory(lua)
endif()

# ZLIB
if (MARSHMALLOW_CONTRIB_ZLIB)
	add_subdirectory(zlib)
endif()

# GLPNG
if(OPENGL_FOUND)
	set(GLPNG_LIBRARIES marshmallow_glpng)
	set(GLPNG_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/glpng/include")
	add_subdirectory(glpng)
	set(GLPNG_LIBRARIES ${GLPNG_LIBRARIES} PARENT_SCOPE)
	set(GLPNG_INCLUDE_DIR ${GLPNG_INCLUDE_DIR} PARENT_SCOPE)
endif()
