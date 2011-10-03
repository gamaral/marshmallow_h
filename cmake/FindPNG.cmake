# - Find the native PNG includes and library
#
# This module defines
#  PNG_INCLUDE_DIR, where to find png.h, etc.
#  PNG_LIBRARIES, the libraries to link against to use PNG.
#  PNG_DEFINITIONS - You should add_definitons(${PNG_DEFINITIONS}) before compiling code that includes png library files.
#  PNG_FOUND, If false, do not try to use PNG.
# also defined, but not for general use are
#  PNG_LIBRARY, where to find the PNG library.
# None of the above will be defined unles zlib can be found.
# PNG depends on Zlib

#=============================================================================
# Copyright 2000-2009 Kitware, Inc., Insight Software Consortium
# All rights reserved.
# Copyright 2011 Marshmallow Engine.
# Copyright 2011 Guillermo A. Amaral B. (gamaral) <g@maral.me>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this
# * list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice,
# * this list of conditions and the following disclaimer in the documentation
# * and/or other materials provided with the distribution.
#
# * Neither the names of Kitware, Inc., the Insight Software Consortium, nor the
# * names of their contributors may be used to endorse or promote products
# * derived from this software without specific prior written  permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
#    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
#    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
#    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
#    OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
#    SUCH DAMAGE.
#
#=============================================================================

if(MARSHMALLOW_CONTRIB_LIBPNG)
  set(PNG_FOUND ON)
  set(PNG_PNG_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/contrib/libpng/
                          ${PROJECT_BINARY_DIR}/contrib/libpng/)
  if(BUILD_SHARED_LIBS)
    set(PNG_DEFINITIONS -DPNG_STATIC)
  endif()
  set(PNG_LIBRARY marshmallow_libpng)
else()
  if(PNG_FIND_QUIETLY)
    set(_FIND_ZLIB_ARG QUIET)
  endif(PNG_FIND_QUIETLY)
  find_package(ZLIB ${_FIND_ZLIB_ARG})

  if(ZLIB_FOUND)
    find_path(PNG_PNG_INCLUDE_DIR png.h
    /usr/local/include/libpng             # OpenBSD
    )

    set(PNG_NAMES ${PNG_NAMES} png libpng png15 libpng15 png15d libpng15d png14 libpng14 png14d libpng14d png12 libpng12 png12d libpng12d)
    find_library(PNG_LIBRARY NAMES ${PNG_NAMES})

    if(PNG_LIBRARY AND PNG_PNG_INCLUDE_DIR)
      set(PNG_FOUND ON)
    endif()
  endif(ZLIB_FOUND)
endif()

if (PNG_LIBRARY AND PNG_PNG_INCLUDE_DIR)
    # png.h includes zlib.h. Sigh.
    SET(PNG_INCLUDE_DIR ${PNG_PNG_INCLUDE_DIR} ${ZLIB_INCLUDE_DIR} )
    SET(PNG_LIBRARIES ${PNG_LIBRARY} ${ZLIB_LIBRARY})

    if (CYGWIN)
      if(BUILD_SHARED_LIBS)
         # No need to define PNG_USE_DLL here, because it's default for Cygwin.
      else(BUILD_SHARED_LIBS)
        SET (PNG_DEFINITIONS -DPNG_STATIC)
      endif(BUILD_SHARED_LIBS)
    endif (CYGWIN)
endif (PNG_LIBRARY AND PNG_PNG_INCLUDE_DIR)


mark_as_advanced(PNG_PNG_INCLUDE_DIR PNG_INCLUDE_DIR PNG_LIBRARY PNG_FOUND PNG_DEFINITIONS PNG_LIBRARIES)

