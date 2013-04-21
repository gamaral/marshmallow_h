project(marshmallow_zlib C)

include(CheckTypeSize)
include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckCSourceCompiles)

option(ZLIB_ASM686 "Enable building i686 assembly implementation")
option(ZLIB_AMD64  "Enable building amd64 assembly implementation")
mark_as_advanced(ZLIB_ASM686 ZLIB_AMD64)

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

set(ZLIB_ASMS)
if(CMAKE_COMPILER_IS_GNUCC)
    if(ZLIB_ASM686)
        set(ZLIB_ASMS code/contrib/asm686/match.S)
    elseif (ZLIB_AMD64)
        set(ZLIB_ASMS code/contrib/amd64/amd64-match.S)
    endif ()

    if(ZLIB_ASMS)
        add_definitions(-DASMV)
        set_source_files_properties(${ZLIB_ASMS} PROPERTIES LANGUAGE C COMPILE_FLAGS -DNO_UNDERLINE)
    endif()
elseif(MSVC)
    if(ZLIB_ASM686)
        ENABLE_LANGUAGE(ASM_MASM)
        set(ZLIB_ASMS code/contrib/masmx86/inffas32.asm
                      code/contrib/masmx86/match686.asm
        )
    elseif (ZLIB_AMD64)
        ENABLE_LANGUAGE(ASM_MASM)
        set(ZLIB_ASMS code/contrib/masmx64/gvmat64.asm
                      code/contrib/masmx64/inffasx64.asm
        )
    endif()

    if(ZLIB_ASMS)
        add_definitions(-DASMV -DASMINF)
    endif()
endif()

if(BUILD_SHARED_LIBS)
	add_definitions(-DZLIB_DLL)
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/code/zconf.h.cmakein
               ${CMAKE_CURRENT_BINARY_DIR}/code/zconf.h @ONLY)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/code)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/code)

#============================================================================
# zlib
#============================================================================

set(ZLIB_PUBLIC_HDRS
    ${CMAKE_CURRENT_BINARY_DIR}/code/zconf.h
    code/zlib.h
)
set(ZLIB_PRIVATE_HDRS
    code/crc32.h
    code/deflate.h
    code/gzguts.h
    code/inffast.h
    code/inffixed.h
    code/inflate.h
    code/inftrees.h
    code/trees.h
    code/zutil.h
)
set(ZLIB_SRCS
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

if(WIN32 AND BUILD_SHARED_LIBS)
    list(APPEND ZLIB_SRCS code/win32/zlib1.rc)
endif()

add_library(${ZLIB_LIBRARY} ${ZLIB_SRCS} ${ZLIB_ASMS} ${ZLIB_PUBLIC_HDRS} ${ZLIB_PRIVATE_HDRS})

install(TARGETS ${ZLIB_LIBRARY}
        LIBRARY DESTINATION lib COMPONENT runtime
        ARCHIVE DESTINATION lib COMPONENT development)

