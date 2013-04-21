project(marshmallow_tinyxml2)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_definitions(-DDEBUG)
endif()

set(MARSHMALLOW_TINYXML2_SRCS code/tinyxml2.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/code)

# TinyXML2 only works as static on Windows since it doesn't generate an import
# library (.lib)
set(MARSHMALLOW_TINYXML2_LIBTYPE)
if(WIN32 AND BUILD_SHARED_LIBS)
	set(MARSHMALLOW_TINYXML2_LIBTYPE "STATIC")
endif()

add_library(${TINYXML2_LIBRARY} ${MARSHMALLOW_TINYXML2_LIBTYPE} ${MARSHMALLOW_TINYXML2_SRCS})

install(TARGETS ${TINYXML2_LIBRARY}
        LIBRARY DESTINATION lib COMPONENT runtime
        ARCHIVE DESTINATION lib COMPONENT development)

