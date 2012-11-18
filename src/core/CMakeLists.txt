add_definitions(-DMARSHMALLOW_CORE_LIBRARY)

file(GLOB MARSHMALLOW_CORE_SRCS *.cpp)
set(MARSHMALLOW_CORE_LIBS "")

# TINYXML
set(MARSHMALLOW_CORE_LIBS ${MARSHMALLOW_CORE_LIBS} ${TINYXML_LIBRARIES})

# Zlib
include_directories(${ZLIB_INCLUDE_DIR})
set(MARSHMALLOW_CORE_LIBS ${MARSHMALLOW_CORE_LIBS} ${ZLIB_LIBRARIES})

# platform specific implementations
if(MARSHMALLOW_WII)
	find_library(OGC_LIBRARY
	    NAMES ogc
	    PATHS ${DEVKITPRO}/libogc/lib/wii
	)
	set(MARSHMALLOW_CORE_SRCS ${MARSHMALLOW_CORE_SRCS} wii/platform.cpp)
	set(MARSHMALLOW_CORE_LIBS ${MARSHMALLOW_CORE_LIBS} ${OGC_LIBRARY})
elseif(UNIX)
	set(MARSHMALLOW_CORE_SRCS ${MARSHMALLOW_CORE_SRCS} unix/platform.cpp)
elseif(WIN32)
	set(MARSHMALLOW_CORE_SRCS ${MARSHMALLOW_CORE_SRCS} win32/platform.cpp)
	set(MARSHMALLOW_CORE_LIBS ${MARSHMALLOW_CORE_LIBS} Winmm)
endif()

add_library(marshmallow_core ${MARSHMALLOW_CORE_SRCS})

target_link_libraries(marshmallow_core ${MARSHMALLOW_CORE_LIBS})

install(TARGETS marshmallow_core
        LIBRARY DESTINATION lib COMPONENT runtime
        ARCHIVE DESTINATION lib COMPONENT development)

