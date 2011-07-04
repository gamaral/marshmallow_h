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

# PNG
find_package(PNG)
add_definitions(${PNG_DEFINITIONS})

include_directories(${PNG_INCLUDE_DIR}
                    ${OPENGL_INCLUDE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/include/)


set(MARSHMALLOW_GLPNG_SRCS src/glpng.c)

if(CMAKE_COMPILER_IS_GNUCC)
	if(BUILD_SHARED_LIBS)
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
	else()
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,")
	endif()
endif()

add_library(marshmallow_glpng STATIC ${MARSHMALLOW_GLPNG_SRCS})

target_link_libraries(marshmallow_glpng ${ZLIB_LIBRARIES}
                                        ${PNG_LIBRARIES}
                                        ${OPENGL_LIBRARIES})

