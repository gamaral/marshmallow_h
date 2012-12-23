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
# NVORBISIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# The views and conclusions contained in the software and documentation are those of the
# authors and should not be interpreted as representing official policies, either expressed
# or implied, of Marshmallow Engine.
#
###############################################################################
# Find VORBIS
###############################################################################
#
#  VORBIS_FOUND
#  VORBIS_INCLUDE_DIRS
#  VORBIS_LIBRARIES
#
#  VORBISCODEC_FOUND
#  VORBISCODEC_INCLUDE_DIR
#  VORBISCODEC_LIBRARY
#
#  VORBISFILE_FOUND
#  VORBISFILE_INCLUDE_DIR
#  VORBISFILE_LIBRARY
#
###############################################################################

find_path(VORBIS_VORBIS_INCLUDE_DIR vorbis/codec.h
	HINTS $ENV{VORBISDIR}
	PATH_SUFFIXES include
	PATHS ~/Library/Frameworks
	      /Library/Frameworks
	      /usr/local
	      /usr
)

find_path(VORBIS_FILE_INCLUDE_DIR vorbis/vorbisfile.h
	HINTS $ENV{VORBISDIR}
	PATH_SUFFIXES include
	PATHS ~/Library/Frameworks
	      /Library/Frameworks
	      /usr/local
	      /usr
)

find_library(VORBIS_VORBIS_LIBRARY
	vorbis
	HINTS $ENV{VORBISDIR}
	PATH_SUFFIXES lib64 lib
	PATHS ~/Library/Frameworks
	      /Library/Frameworks
	      /usr/local
	      /usr
)

find_library(VORBIS_FILE_LIBRARY
	vorbisfile
	HINTS $ENV{VORBISDIR}
	PATH_SUFFIXES lib64 lib
	PATHS ~/Library/Frameworks
	      /Library/Frameworks
	      /usr/local
	      /usr
)

set(VORBIS_INCLUDE_DIRS ${VORBIS_VORBIS_INCLUDE_DIR}
                        ${VORBIS_FILE_INCLUDE_DIR}
)

set(VORBIS_LIBRARIES ${VORBIS_VORBIS_LIBRARY}
                     ${VORBIS_FILE_LIBRARY}
)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(VORBIS DEFAULT_MSG VORBIS_VORBIS_LIBRARY
                                                     VORBIS_FILE_LIBRARY
                                                     VORBIS_LIBRARIES
)

mark_as_advanced(VORBIS_LIBRARIES VORBIS_INCLUDE_DIRS
                                  VORBIS_VORBIS_LIBRARY
                                  VORBIS_VORBIS_INCLUDE_DIR
                                  VORBIS_FILE_LIBRARY
                                  VORBIS_FILE_INCLUDE_DIR
)
