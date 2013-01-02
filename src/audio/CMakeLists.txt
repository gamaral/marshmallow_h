###################################################################### OPTIONS #

option(MARSHMALLOW_AUDIO_DUMMY  "Use Dummy Audio Backend"  ON)
option(MARSHMALLOW_AUDIO_ALSA   "Use ALSA Audio Backend"   OFF)
option(MARSHMALLOW_AUDIO_OPENAL "Use OpenAL Audio Backend" OFF)

################################################################################

add_definitions(-DMARSHMALLOW_AUDIO_LIBRARY)

set(MARSHMALLOW_AUDIO_SRCS pcm.cpp
                           track.cpp
                           oggcodec.cpp
                           wavecodec.cpp
)

set(MARSHMALLOW_AUDIO_LIBS marshmallow_core
                           marshmallow_audio_backend
                           ${OGG_LIBRARY}
                           ${VORBIS_LIBRARIES}
)

include_directories(${OGG_INCLUDE_DIR} ${VORBIS_INCLUDE_DIRS})

if(MARSHMALLOW_AUDIO_ALSA)
	add_subdirectory(alsa)
elseif(MARSHMALLOW_AUDIO_OPENAL)
	add_subdirectory(openal)
elseif(MARSHMALLOW_AUDIO_DUMMY)
	add_subdirectory(dummy)
endif()

################################################################################

add_library(marshmallow_audio ${MARSHMALLOW_AUDIO_SRCS})

target_link_libraries(marshmallow_audio ${MARSHMALLOW_AUDIO_LIBS})

install(TARGETS marshmallow_audio
        LIBRARY DESTINATION lib COMPONENT runtime
        ARCHIVE DESTINATION lib COMPONENT development
)

