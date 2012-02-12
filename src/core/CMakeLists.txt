add_definitions(-DCORE_LIBRARY)

file(GLOB MARSHMALLOW_CORE_SRCS *.cpp)
set(MARSHMALLOW_CORE_LIBS "")

# TINYXML
include_directories(${TINYXML_INCLUDE_DIR})
set(MARSHMALLOW_CORE_LIBS ${MARSHMALLOW_CORE_LIBS} ${TINYXML_LIBRARIES})

# Zlib
include_directories(${ZLIB_INCLUDE_DIR})
set(MARSHMALLOW_CORE_LIBS ${MARSHMALLOW_CORE_LIBS} ${ZLIB_LIBRARIES})

# platform specific implementations
if(UNIX)
	set(MARSHMALLOW_CORE_SRCS ${MARSHMALLOW_CORE_SRCS} unix/platform.cpp)
else(WIN32)
	set(MARSHMALLOW_CORE_SRCS ${MARSHMALLOW_CORE_SRCS} win32/platform.cpp)
	set(MARSHMALLOW_CORE_LIBS ${MARSHMALLOW_CORE_LIBS} Winmm)
endif()

add_library(marshmallow_core ${MARSHMALLOW_CORE_SRCS})

target_link_libraries(marshmallow_core ${MARSHMALLOW_CORE_LIBS})

