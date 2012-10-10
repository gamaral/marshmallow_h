set(CMAKE_SYSTEM_NAME QNX)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR $ENV{QNX_TARGET_PLATFORM})

set(QNX_HOST   $ENV{QNX_HOST}   CACHE INTERNAL "" FORCE)
set(QNX_TARGET $ENV{QNX_TARGET} CACHE INTERNAL "" FORCE)

if ("${QNX_HOST}" STREQUAL "" OR "${QNX_TARGET}" STREQUAL "")
	message(FATAL_ERROR "\n*** Invalid toolchain environment ***\n"
	                    "Please source the toolchain environment file.\n")
elseif ("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "")
	set(CMAKE_SYSTEM_PROCESSOR "armle-v7")
	message(STATUS "\n*** No platform specified (QNX_TARGET_PLATFORM) "
	    "defaulting to armle-v7 ***\n")
endif()

set(BUILD_UNIT_TESTS OFF CACHE BOOL "")

if ("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "armle-v7")
	set(CMAKE_C_COMPILER   ${QNX_HOST}/usr/bin/ntoarmv7-gcc CACHE FILEPATH "" FORCE)
	set(CMAKE_CXX_COMPILER ${QNX_HOST}/usr/bin/ntoarmv7-g++ CACHE FILEPATH "" FORCE)
elseif ("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86")
	set(CMAKE_C_COMPILER   ${QNX_HOST}/usr/bin/ntox86-gcc CACHE FILEPATH "" FORCE)
	set(CMAKE_CXX_COMPILER ${QNX_HOST}/usr/bin/ntox86-g++ CACHE FILEPATH "" FORCE)
endif()

set(CMAKE_FIND_ROOT_PATH ${QNX_HOST}
                         ${QNX_TARGET}
                         ${QNX_TARGET}/${CMAKE_SYSTEM_PROCESSOR}
                         ${QNX_TARGET}/${CMAKE_SYSTEM_PROCESSOR}/usr/lib/qt4)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

