# Copyright 2012 Marshmallow Engine. All rights reserved.
# Copyright 2012 Guillermo A. Amaral B. (gamaral) <g@maral.me>
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

project(marshmallow_vorbis)

# sanity check
if(NOT OGG_FOUND)
	message(FATAL_ERROR "Vorbis requires the Ogg library, and it was not found!")
endif()

set(MARSHMALLOW_VORBIS_SRCS code/lib/analysis.c
                            code/lib/bitrate.c
                            code/lib/block.c
                            code/lib/codebook.c
                            code/lib/envelope.c
                            code/lib/floor0.c
                            code/lib/floor1.c
                            code/lib/info.c
                            code/lib/lookup.c
                            code/lib/lpc.c
                            code/lib/lsp.c
                            code/lib/mapping0.c
                            code/lib/mdct.c
                            code/lib/psy.c
                            code/lib/registry.c
                            code/lib/res0.c
                            code/lib/sharedbook.c
                            code/lib/smallft.c
                            code/lib/synthesis.c
                            code/lib/window.c)

set(MARSHMALLOW_VORBISFILE_SRCS code/lib/vorbisfile.c)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/code/include
                    ${OGG_INCLUDE_DIR})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_definitions(-DDEBUG)
endif()

add_library(marshmallow_vorbis ${MARSHMALLOW_VORBIS_SRCS})
add_library(marshmallow_vorbisfile ${MARSHMALLOW_VORBISFILE_SRCS})

target_link_libraries(marshmallow_vorbis ${OGG_LIBRARY})
target_link_libraries(marshmallow_vorbisfile marshmallow_vorbis)

install(TARGETS marshmallow_vorbis marshmallow_vorbisfile
        LIBRARY DESTINATION lib COMPONENT runtime
        ARCHIVE DESTINATION lib COMPONENT development)

