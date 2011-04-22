# Copyright (C) 2007-2011 Glenn Randers-Pehrson

# This code is released under the libpng license.
# For conditions of distribution and use, see the disclaimer
# and license in png.h

find_package(ZLIB)
include_directories(${ZLIB_INCLUDE_DIR})

mark_as_advanced(LIBPNG_ARG LIBPNG_DEBUG LIBPNG_M_LIBRARY LIBPNG_NO_CONSOLE_IO
    LIBPNG_NO_STDIO BUILD_SHARED_LIBS NOT BUILD_SHARED_LIBS LIBPNG_UNAME_EXECUTABLE)

if(NOT WIN32)
  find_library(LIBPNG_M_LIBRARY
    NAMES m
    PATHS /usr/lib /usr/local/lib
  )
  if(NOT LIBPNG_M_LIBRARY)
    message(STATUS
      "math library 'libm' not found - floating point support disabled")
  endif()
else()
  # not needed on windows
  set(LIBPNG_M_LIBRARY "")
endif()

# COMMAND LINE OPTIONS
if(BUILD_SHARED_LIBS)
  set(CMAKE_C_FLAGS "-fPIC")
else()
  set(CMAKE_C_FLAG "-Wl," )
endif()

option(LIBPNG_NO_CONSOLE_IO "FIXME" YES)
option(LIBPNG_NO_STDIO      "FIXME" YES)
option(LIBPNG_DEBUG         "Build with debug output" NO)
option(LIBPNG_ARG            "FIXME" YES)
#TODO:
# PNG_CONSOLE_IO_SUPPORTED

# maybe needs improving, but currently I don't know when we can enable what :)
set(png_asm_tmp "OFF")
if(NOT WIN32)
  find_program(LIBPNG_UNAME_EXECUTABLE NAMES uname PATHS /bin /usr/bin /usr/local/bin)
  if(LIBPNG_UNAME_EXECUTABLE)
    exec_program(${LIBPNG_UNAME_EXECUTABLE}
                 ARGS --machine OUTPUT_VARIABLE uname_output)
    if("uname_output" MATCHES "^.*i[1-9]86.*$")
      set(png_asm_tmp "ON")
    else("uname_output" MATCHES "^.*i[1-9]86.*$")
      set(png_asm_tmp "OFF")
    endif("uname_output" MATCHES "^.*i[1-9]86.*$")
  endif(LIBPNG_UNAME_EXECUTABLE)
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
  add_definitions(-DLIBPNG_NO_MODULEDEF -D_CRT_SECURE_NO_DEPRECATE)
endif(MSVC)

if(BUILD_SHARED_LIBS OR  NOT MSVC)
  #if building msvc static this has NOT to be defined
  add_definitions(-DZLIB_DLL)
endif()


if(PNG_CONSOLE_IO_SUPPORTED)
  add_definitions(-DPNG_CONSOLE_IO_SUPPORTED)
endif()

if(LIBPNG_NO_CONSOLE_IO)
  add_definitions(-DLIBPNG_NO_CONSOLE_IO)
endif()

if(LIBPNG_NO_STDIO)
  add_definitions(-DLIBPNG_NO_STDIO)
endif()

if(LIBPNG_DEBUG)
  add_definitions(-DLIBPNG_DEBUG)
endif()

if(NOT LIBPNG_M_LIBRARY AND NOT WIN32)
  add_definitions(-DLIBPNG_NO_FLOATING_POINT_SUPPORTED)
endif()

# NOW BUILD OUR TARGET
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(${PNG_LIB_NAME} ${libpng_sources})
target_link_libraries(${PNG_LIB_NAME} ${ZLIB_LIBRARY} ${LIBPNG_M_LIBRARY})

if(BUILD_SHARED_LIBS)
  if(MSVC)
    # msvc does not append 'lib' - do it here to have consistent name
    set_target_properties(${PNG_LIB_NAME} PROPERTIES PREFIX "lib")
    set_target_properties(${PNG_LIB_NAME} PROPERTIES IMPORT_PREFIX "lib")
  endif()
else()
  if(MSVC)
    # msvc does not append 'lib' - do it here to have consistent name
    set_target_properties(${PNG_LIB_NAME} PROPERTIES PREFIX "lib")
  endif()
endif()


if(BUILD_SHARED_LIBS AND WIN32)
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

