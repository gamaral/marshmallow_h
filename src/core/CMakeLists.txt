add_definitions(-DMARSHMALLOW_CORE_LIBRARY)

configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

file(GLOB MARSHMALLOW_CORE_SRCS *.cpp)
set(MARSHMALLOW_CORE_LIBS "")

# TINYXML
set(MARSHMALLOW_CORE_LIBS ${MARSHMALLOW_CORE_LIBS} ${TINYXML_LIBRARY})

# Zlib
include_directories(${ZLIB_INCLUDE_DIR})
set(MARSHMALLOW_CORE_LIBS ${MARSHMALLOW_CORE_LIBS} ${ZLIB_LIBRARY})

# platform specific implementations
if(MARSHMALLOW_WII)
	find_library(OGC_LIBRARY
	    NAMES ogc
	    PATHS ${DEVKITPRO}/libogc/lib/wii
	)
	set(MARSHMALLOW_CORE_SRCS ${MARSHMALLOW_CORE_SRCS} wii/platform.cpp)
	set(MARSHMALLOW_CORE_LIBS ${MARSHMALLOW_CORE_LIBS} ${OGC_LIBRARY})
elseif(UNIX)
	configure_file(
	    ${CMAKE_CURRENT_SOURCE_DIR}/unix/environment.h
	    ${CMAKE_CURRENT_BINARY_DIR}/environment.h COPYONLY
	)

	set(MARSHMALLOW_CORE_SRCS ${MARSHMALLOW_CORE_SRCS} unix/platform.cpp)
elseif(WIN32)
	configure_file(
	    ${CMAKE_CURRENT_SOURCE_DIR}/win32/environment.h
	    ${CMAKE_CURRENT_BINARY_DIR}/environment.h COPYONLY
	)

	set(MARSHMALLOW_CORE_SRCS ${MARSHMALLOW_CORE_SRCS} win32/platform.cpp)
	set(MARSHMALLOW_CORE_LIBS ${MARSHMALLOW_CORE_LIBS} Winmm)
endif()

add_library(marshmallow_core ${MARSHMALLOW_CORE_SRCS})

target_link_libraries(marshmallow_core ${MARSHMALLOW_CORE_LIBS})

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/environment.h
        DESTINATION include/marshmallow_h/core
        COMPONENT development)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/config.h
        DESTINATION include/marshmallow_h/core
        COMPONENT development)

install(TARGETS marshmallow_core
        LIBRARY DESTINATION lib COMPONENT runtime
        ARCHIVE DESTINATION lib COMPONENT development)

