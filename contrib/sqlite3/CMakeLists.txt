project(marshmallow_sqlite3)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_definitions(-DDEBUG)
endif()

set(MARSHMALLOW_SQLITE_SRCS code/sqlite3.c)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/code)

add_library(marshmallow_sqlite3 ${MARSHMALLOW_SQLITE_SRCS})

install(TARGETS marshmallow_sqlite3
        LIBRARY DESTINATION lib COMPONENT runtime
        ARCHIVE DESTINATION lib COMPONENT development)

