set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true)

# Copyright (C) 2007-2011 Glenn Randers-Pehrson

# This code is released under the libpng license.
# For conditions of distribution and use, see the disclaimer
# and license in png.h

# needed packages
find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIR})

if(NOT WIN32)
  find_library(M_LIBRARY
    NAMES m
    PATHS /usr/lib /usr/local/lib
  )
  if(NOT M_LIBRARY)
    message(STATUS
      "math library 'libm' not found - floating point support disabled")
  endif()
else()
  # not needed on windows
  set(M_LIBRARY "")
endif()

# COMMAND LINE OPTIONS
if(BUILD_SHARED_LIBS)
  option(PNG_SHARED "Build shared lib" ON)
  option(PNG_STATIC "Build static lib" OFF)
  set(CMAKE_C_FLAGS "-fPIC")
else()
  option(PNG_SHARED "Build shared lib" OFF)
  option(PNG_STATIC "Build static lib" ON)
  set(CMAKE_C_FLAG "-Wl," )
endif()

option(PNG_NO_CONSOLE_IO "FIXME" YES)
option(PNG_NO_STDIO      "FIXME" YES)
option(PNG_DEBUG         "Build with debug output" NO)
option(PNGARG            "FIXME" YES)
#TODO:
# PNG_CONSOLE_IO_SUPPORTED

# maybe needs improving, but currently I don't know when we can enable what :)
set(png_asm_tmp "OFF")
if(NOT WIN32)
  find_program(uname_executable NAMES uname PATHS /bin /usr/bin /usr/local/bin)
  if(uname_executable)
    exec_program(${uname_executable}
                 ARGS --machine OUTPUT_VARIABLE uname_output)
    if("uname_output" MATCHES "^.*i[1-9]86.*$")
      set(png_asm_tmp "ON")
    else("uname_output" MATCHES "^.*i[1-9]86.*$")
      set(png_asm_tmp "OFF")
    endif("uname_output" MATCHES "^.*i[1-9]86.*$")
  endif(uname_executable)
else()
  # this env var is normally only set on win64
  set(TEXT "ProgramFiles(x86)")
  if("$ENV{${TEXT}}" STREQUAL "")
    set(png_asm_tmp "ON")
  endif("$ENV{${TEXT}}" STREQUAL "")
endif()

# SET LIBNAME
set(PNG_LIB_NAME marshmallow_libpng)

# Use the prebuilt pnglibconf.h file from the scripts folder
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/scripts/pnglibconf.h.prebuilt
               ${CMAKE_CURRENT_BINARY_DIR}/pnglibconf.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# OUR SOURCES
set(libpng_public_hdrs
  png.h
  pngconf.h
  ${CMAKE_CURRENT_BINARY_DIR}/pnglibconf.h
)
set(libpng_sources
  ${libpng_public_hdrs}
  pngdebug.h
  pnginfo.h
  pngpriv.h
  pngstruct.h
  png.c
  pngerror.c
  pngget.c
  pngmem.c
  pngpread.c
  pngread.c
  pngrio.c
  pngrtran.c
  pngrutil.c
  pngset.c
  pngtrans.c
  pngwio.c
  pngwrite.c
  pngwtran.c
  pngwutil.c
)
# SOME NEEDED DEFINITIONS

add_definitions(-DPNG_CONFIGURE_LIBPNG)

if(_AIX)
  add_definitions(-D_ALL_SOURCE)
endif(_AIX)

if(MSVC)
  add_definitions(-DPNG_NO_MODULEDEF -D_CRT_SECURE_NO_DEPRECATE)
endif(MSVC)

if(PNG_SHARED OR  NOT MSVC)
  #if building msvc static this has NOT to be defined
  add_definitions(-DZLIB_DLL)
endif()


if(PNG_CONSOLE_IO_SUPPORTED)
  add_definitions(-DPNG_CONSOLE_IO_SUPPORTED)
endif()

if(PNG_NO_CONSOLE_IO)
  add_definitions(-DPNG_NO_CONSOLE_IO)
endif()

if(PNG_NO_STDIO)
  add_definitions(-DPNG_NO_STDIO)
endif()

if(PNG_DEBUG)
  add_definitions(-DPNG_DEBUG)
endif()

if(NOT M_LIBRARY AND NOT WIN32)
  add_definitions(-DPNG_NO_FLOATING_POINT_SUPPORTED)
endif()

# NOW BUILD OUR TARGET
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${ZLIB_INCLUDE_DIR})

if(PNG_SHARED)
  add_library(${PNG_LIB_NAME} SHARED ${libpng_sources})
  if(MSVC)
    # msvc does not append 'lib' - do it here to have consistent name
    set_target_properties(${PNG_LIB_NAME} PROPERTIES PREFIX "lib")
    set_target_properties(${PNG_LIB_NAME} PROPERTIES IMPORT_PREFIX "lib")
  endif()
  target_link_libraries(${PNG_LIB_NAME} ${ZLIB_LIBRARY} ${M_LIBRARY})
endif()

if(PNG_STATIC)
# does not work without changing name
  add_library(${PNG_LIB_NAME} STATIC ${libpng_sources})
  if(MSVC)
    # msvc does not append 'lib' - do it here to have consistent name
    set_target_properties(${PNG_LIB_NAME} PROPERTIES PREFIX "lib")
  endif()
  target_link_libraries(${PNG_LIB_NAME} ${ZLIB_LIBRARY} ${M_LIBRARY})
endif()


if(PNG_SHARED AND WIN32)
  set_target_properties(${PNG_LIB_NAME} PROPERTIES DEFINE_SYMBOL PNG_BUILD_DLL)
endif()

#
# Set a variable with CMake code which:
# Creates a symlink from src to dest (if possible) or alternatively
# copies if different.
macro(_png_generate_symlink_code CODE SRC DEST)
  if(WIN32 AND NOT CYGWIN)
    set(_png_gsc_message "Copying ${SRC} to ${DEST} if needed")
    set(_png_gsc_operation "copy_if_different")
  else()
    set(_png_gsc_message "Symlinking ${SRC} to ${DEST}")
    set(_png_gsc_operation "create_symlink")
  endif()

  set(${CODE} "
    message(STATUS \"${_png_gsc_message}\")
    execute_process(COMMAND \${CMAKE_COMMAND} -E ${_png_gsc_operation}
      \"${SRC}\" \"${DEST}\")
    ")
endmacro()

# CREATE PKGCONFIG FILES
# we use the same files like ./configure, so we have to set its vars
set(LIBS        "-lz -lm")

