project(marshmallow_libogg)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_definitions(-DDEBUG)
endif()

include(CheckIncludeFile)
check_include_file(inttypes.h  INCLUDE_INTTYPES_H)
check_include_file(stdint.h    INCLUDE_STDINT_H)
check_include_file(sys/types.h INCLUDE_SYS_TYPES_H)

set(SIZE16  int16_t)
set(USIZE16 uint16_t)
set(SIZE32  int32_t)
set(USIZE32 uint32_t)
set(SIZE64  int64_t)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/code/include/ogg/config_types.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/code/include/ogg/config_types.h @ONLY)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/code/include
                    ${CMAKE_CURRENT_BINARY_DIR}/code/include)

set(MARSHMALLOW_OGG_SRCS code/src/bitwise.c
                         code/src/framing.c
)

# Windows Module-Definition File
if (BUILD_SHARED_LIBS AND WIN32)
	list(APPEND MARSHMALLOW_OGG_SRCS code/win32/ogg.def)
endif()

add_library(${OGG_LIBRARY} ${MARSHMALLOW_OGG_SRCS})

install(TARGETS ${OGG_LIBRARY}
        LIBRARY DESTINATION lib COMPONENT runtime
        ARCHIVE DESTINATION lib COMPONENT development)

