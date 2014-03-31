add_definitions(-DMARSHMALLOW_CORE_LIBRARY)

set(MARSHMALLOW_CORE_SRCS)
set(MARSHMALLOW_CORE_LIBS)

set(MARSHMALLOW_CORE_CONFIG_H "${CMAKE_CURRENT_BINARY_DIR}/config.h")
configure_file("config.h.in" ${MARSHMALLOW_CORE_CONFIG_H})

# platform specific implementations and environment definitions
set(MARSHMALLOW_CORE_ENVIRONMENT_H "${CMAKE_CURRENT_BINARY_DIR}/environment.h")
if(UNIX)
	configure_file(
	    "${CMAKE_CURRENT_SOURCE_DIR}/unix/environment.h"
	    ${MARSHMALLOW_CORE_ENVIRONMENT_H} COPYONLY
	)

	list(APPEND MARSHMALLOW_CORE_SRCS "unix/platform.cpp")
elseif(WIN32)
	configure_file(
	    "${CMAKE_CURRENT_SOURCE_DIR}/win32/environment.h"
	    ${MARSHMALLOW_CORE_ENVIRONMENT_H} COPYONLY
	)

	list(APPEND MARSHMALLOW_CORE_SRCS "win32/platform.cpp")
	list(APPEND MARSHMALLOW_CORE_LIBS "Winmm")
else()
	message(FATAL_ERROR "No environment definitions, unknown platform!")
endif()

file(GLOB MARSHMALLOW_CORE_SRCS_CPP "*.cpp")
list(APPEND MARSHMALLOW_CORE_SRCS ${MARSHMALLOW_CORE_SRCS_CPP})

set(MARSHMALLOW_CORE_HDRS ${MARSHMALLOW_CORE_CONFIG_H}
                          ${MARSHMALLOW_CORE_ENVIRONMENT_H}
)

# zlib
include_directories(${ZLIB_INCLUDE_DIR})
list(APPEND MARSHMALLOW_CORE_LIBS ${ZLIB_LIBRARY})

# unix clock_gettime check
if (UNIX)
	include(CheckLibraryExists)
	check_library_exists(rt clock_gettime "time.h" HAVE_CLOCK_GETTIME)

	# rt
	if(HAVE_CLOCK_GETTIME)
		list(APPEND MARSHMALLOW_CORE_LIBS "rt")

		find_package(Threads REQUIRED)
		list(APPEND MARSHMALLOW_CORE_LIBS ${CMAKE_THREAD_LIBS_INIT})
	endif()
endif()

add_library(marshmallow_core ${MARSHMALLOW_CORE_SRCS} ${MARSHMALLOW_CORE_HDRS})

target_link_libraries(marshmallow_core ${MARSHMALLOW_CORE_LIBS})

install(FILES ${MARSHMALLOW_CORE_ENVIRONMENT_H}
        DESTINATION "include/marshmallow_h/core"
        COMPONENT development)

install(FILES ${MARSHMALLOW_CORE_CONFIG_H}
        DESTINATION "include/marshmallow_h/core"
        COMPONENT development)

install(TARGETS marshmallow_core
        LIBRARY DESTINATION lib COMPONENT runtime
        ARCHIVE DESTINATION lib COMPONENT development)

