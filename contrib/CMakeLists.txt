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

add_subdirectory(eastl)
set(EASTL_LIBRARIES marshmallow_eastl PARENT_SCOPE)
set(EASTL_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/eastl/include"
    PARENT_SCOPE)

add_subdirectory(tinyxml)
set(TINYXML_LIBRARIES marshmallow_tinyxml PARENT_SCOPE)
set(TINYXML_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tinyxml/include"
    PARENT_SCOPE)

add_subdirectory(box2d)
set(BOX2D_LIBRARIES marshmallow_box2d PARENT_SCOPE)
set(BOX2D_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/box2d/include"
    PARENT_SCOPE)

if (MARSHMALLOW_CONTRIB_ZLIB AND ZLIB_FOUND)
	add_subdirectory(zlib)
endif()

if(MARSHMALLOW_CONTRIB_LIBPNG AND PNG_FOUND)
	add_subdirectory(libpng)
endif()

if(OPENGL_FOUND)
	add_subdirectory(glpng)
endif()

