project(marshmallow_zlib)

include(CheckTypeSize)
include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckCSourceCompiles)

if (BUILD_SHARED_LIBS)
    add_definitions(-DZLIB_DLL)
endif()

check_include_file(sys/types.h HAVE_SYS_TYPES_H)
check_include_file(stdint.h    HAVE_STDINT_H)
check_include_file(stddef.h    HAVE_STDDEF_H)

#
# Check to see if we have large file support
#
set(CMAKE_REQUIRED_DEFINITIONS -D_LARGEFILE64_SOURCE=1)
# We add these other definitions here because CheckTypeSize.cmake
# in CMake 2.4.x does not automatically do so and we want
# compatibility with CMake 2.4.x.
if(HAVE_SYS_TYPES_H)
    list(APPEND CMAKE_REQUIRED_DEFINITIONS -DHAVE_SYS_TYPES_H)
endif()
if(HAVE_STDINT_H)
    list(APPEND CMAKE_REQUIRED_DEFINITIONS -DHAVE_STDINT_H)
endif()
if(HAVE_STDDEF_H)
    list(APPEND CMAKE_REQUIRED_DEFINITIONS -DHAVE_STDDEF_H)
endif()
check_type_size(off64_t OFF64_T)
if(HAVE_OFF64_T)
   add_definitions(-D_LARGEFILE64_SOURCE=1)
endif()
set(CMAKE_REQUIRED_DEFINITIONS) # clear variable

#
# Check for fseeko
#
check_function_exists(fseeko HAVE_FSEEKO)
if(NOT HAVE_FSEEKO)
    add_definitions(-DNO_FSEEKO)
endif()

#
# Check for unistd.h
#
check_include_file(unistd.h Z_HAVE_UNISTD_H)

if(MSVC)
    set(CMAKE_DEBUG_POSTFIX "d")
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/code/zconf.h.cmakein
               ${CMAKE_CURRENT_BINARY_DIR}/code/zconf.h @ONLY)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/code)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/code)


#============================================================================
# zlib
#============================================================================

set(ZLIB_SRCS
    ${CMAKE_CURRENT_BINARY_DIR}/code/zconf.h
    code/zlib.h
    code/crc32.h
    code/deflate.h
    code/gzguts.h
    code/inffast.h
    code/inffixed.h
    code/inflate.h
    code/inftrees.h
    code/trees.h
    code/zutil.h
    code/adler32.c
    code/compress.c
    code/crc32.c
    code/deflate.c
    code/gzclose.c
    code/gzlib.c
    code/gzread.c
    code/gzwrite.c
    code/inflate.c
    code/infback.c
    code/inftrees.c
    code/inffast.c
    code/trees.c
    code/uncompr.c
    code/zutil.c
)

# parse the full version number from zlib.h and include in ZLIB_FULL_VERSION
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/code/zlib.h _zlib_h_contents)
string(REGEX REPLACE ".*#define[ \t]+ZLIB_VERSION[ \t]+\"([0-9A-Za-z.]+)\".*"
    "\\1" ZLIB_FULL_VERSION ${_zlib_h_contents})

add_library(${ZLIB_LIBRARY} ${ZLIB_SRCS})

