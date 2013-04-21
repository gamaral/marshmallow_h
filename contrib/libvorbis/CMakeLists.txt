project(marshmallow_libvorbis)

# sanity check
if(NOT OGG_FOUND)
	message(FATAL_ERROR "libvorbis requires the libogg library, and it wasn't found!")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_definitions(-DDEBUG)
endif()

set(MARSHMALLOW_VORBIS_SRCS code/lib/analysis.c
                            code/lib/bitrate.c
                            code/lib/block.c
                            code/lib/codebook.c
                            code/lib/envelope.c
                            code/lib/floor0.c
                            code/lib/floor1.c
                            code/lib/info.c
                            code/lib/lookup.c
                            code/lib/lpc.c
                            code/lib/lsp.c
                            code/lib/mapping0.c
                            code/lib/mdct.c
                            code/lib/psy.c
                            code/lib/registry.c
                            code/lib/res0.c
                            code/lib/sharedbook.c
                            code/lib/smallft.c
                            code/lib/synthesis.c
                            code/lib/window.c
)

set(MARSHMALLOW_VORBISFILE_SRCS code/lib/vorbisfile.c)

# Windows Module-Definition Files
if (WIN32 AND BUILD_SHARED_LIBS)
	list(APPEND MARSHMALLOW_VORBIS_SRCS code/lib/vorbisenc.c
	                                    code/win32/vorbis.def
	)
	list(APPEND MARSHMALLOW_VORBISFILE_SRCS code/win32/vorbisfile.def)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/code/include
                    ${OGG_INCLUDE_DIR}
)

add_library(${VORBIS_VORBIS_LIBRARY} ${MARSHMALLOW_VORBIS_SRCS})
add_library(${VORBIS_FILE_LIBRARY} ${MARSHMALLOW_VORBISFILE_SRCS})

target_link_libraries(${VORBIS_VORBIS_LIBRARY} ${OGG_LIBRARY} ${M_LIBRARY})
target_link_libraries(${VORBIS_FILE_LIBRARY} ${VORBIS_VORBIS_LIBRARY})

install(TARGETS ${VORBIS_VORBIS_LIBRARY} ${VORBIS_FILE_LIBRARY}
        LIBRARY DESTINATION lib COMPONENT runtime
        ARCHIVE DESTINATION lib COMPONENT development)

