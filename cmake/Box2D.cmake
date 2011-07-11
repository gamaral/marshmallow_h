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
#=============================================================================
#
#  BOX2D_INCLUDE_DIRS
#  BOX2D_LIBRARIES
#  BOX2D_FOUND
#
#=============================================================================

if(MARSHMALLOW_WITH_BOX2D)
    set(BOX2D_FOUND ON CACHE BOOL "")

    set(BOX2D_BASE "${PROJECT_SOURCE_DIR}/contrib/box2d/code/Box2D")
    set(BOX2D_INCLUDE_DIR ${BOX2D_BASE} CACHE STRING "")
    set(BOX2D_LIBRARY marshmallow_box2d CACHE STRING "")

    set(BOX2D_INCLUDE_DIRS ${BOX2D_INCLUDE_DIR} CACHE STRING "")
    set(BOX2D_LIBRARIES ${BOX2D_LIBRARY} CACHE STRING "")

    include_directories(${BOX2D_INCLUDE_DIRS})

    message(STATUS "Including Box2D from ${BOX2D_BASE}")
else(MARSHMALLOW_WITH_BOX2D)
    set(BOX2D_FOUND OFF CACHE BOOL "")
endif(MARSHMALLOW_WITH_BOX2D)

mark_as_advanced(BOX2D_FOUND BOX2D_INCLUDE_DIR BOX2D_LIBRARY BOX2D_INCLUDE_DIRS BOX2D_LIBRARIES)

