# Copyright 2012 Marshmallow Engine. All rights reserved.
# Copyright 2012 Guillermo A. Amaral B. (gamaral) <g@maral.me>.
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

if(DEFINED CMAKE_CROSSCOMPILING)
	return()
endif()

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR powerpc-eabi)

set(DEVKITPPC $ENV{DEVKITPPC} CACHE STRING "" FORCE)
set(DEVKITPRO $ENV{DEVKITPRO} CACHE STRING "" FORCE)

set(CMAKE_C_COMPILER   ${DEVKITPPC}/bin/powerpc-eabi-gcc)
set(CMAKE_CXX_COMPILER ${DEVKITPPC}/bin/powerpc-eabi-g++)
set(CMAKE_C_FLAGS      "-MMD -MP -mrvl -mcpu=750 -meabi -mhard-float")
set(CMAKE_CXX_FLAGS    "-MMD -MP -mrvl -mcpu=750 -meabi -mhard-float")

set(MARSHMALLOW_WII true)

set(CMAKE_FIND_ROOT_PATH ${DEVKITPPC} ${DEVKITPRO} ${DEVKITPPC}/powerpc-eabi ${DEVKITPRO}/libogc)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

include_directories(${DEVKITPRO}/libogc/include)
link_directories(${DEVKITPRO}/libogc/lib/wii)

add_definitions(-DGEKKO)

