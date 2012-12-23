project(marshmallow_libpng)

if(NOT ZLIB_FOUND)
	message(FATAL_ERROR "LibPNG requires Zlib, and it was not found!")
endif()

include_directories(${ZLIB_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/code)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/code)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/code/scripts/pnglibconf.h.prebuilt
               ${CMAKE_CURRENT_BINARY_DIR}/code/pnglibconf.h)

set(PNG_SRCS ${CMAKE_CURRENT_BINARY_DIR}/code/pnglibconf.h
             code/png.h
             code/pngconf.h
             code/pngdebug.h
             code/pnginfo.h
             code/pngpriv.h
             code/pngstruct.h
             code/png.c
             code/pngerror.c
             code/pngget.c
             code/pngmem.c
             code/pngpread.c
             code/pngread.c
             code/pngrio.c
             code/pngrtran.c
             code/pngrutil.c
             code/pngset.c
             code/pngtrans.c
             code/pngwio.c
             code/pngwrite.c
             code/pngwtran.c
             code/pngwutil.c
)

add_definitions(-DPNG_CONFIGURE_LIBPNG)

if(MARSHMALLOW_DEBUG)
	add_definitions(-DLIBPNG_DEBUG)
endif()

if(BUILD_SHARED_LIBS)
	add_definitions(-DZLIB_DLL)
endif()

if(MSVC)
	add_definitions(-DLIBPNG_NO_MODULEDEF -D_CRT_SECURE_NO_DEPRECATE)
endif(MSVC)

#add_definitions(-DPNG_CONSOLE_IO_SUPPORTED)
add_definitions(-DLIBPNG_NO_CONSOLE_IO)
add_definitions(-DLIBPNG_NO_STDIO)

add_library(${PNG_LIBRARY} ${PNG_SRCS})
target_link_libraries(${PNG_LIBRARY} ${ZLIB_LIBRARY} ${M_LIBRARY})

install(TARGETS ${PNG_LIBRARY}
        LIBRARY DESTINATION lib COMPONENT runtime
        ARCHIVE DESTINATION lib COMPONENT development)

