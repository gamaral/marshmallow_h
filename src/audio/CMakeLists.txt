###################################################################### OPTIONS #

option(MARSHMALLOW_AUDIO_DUMMY  "Use Dummy Audio Backend"  ON)
option(MARSHMALLOW_AUDIO_ALSA   "Use ALSA Audio Backend"   OFF)
option(MARSHMALLOW_AUDIO_OPENAL "Use OpenAL Audio Backend" OFF)

################################################################################

add_definitions(-DMARSHMALLOW_AUDIO_LIBRARY)

set(MARSHMALLOW_AUDIO_SRCS track.cpp
                           oggcodec.cpp
                           wavecodec.cpp
)

set(MARSHMALLOW_AUDIO_LIBS marshmallow_core
                           ${OGG_LIBRARY}
                           ${VORBIS_LIBRARIES}
)

if(MARSHMALLOW_AUDIO_ALSA)
	add_subdirectory(alsa)
	set(MARSHMALLOW_AUDIO_LIBS ${MARSHMALLOW_AUDIO_LIBS}
	                           marshmallow_audio_alsa
	)
elseif(MARSHMALLOW_AUDIO_DUMMY)
	add_subdirectory(dummy)
	set(MARSHMALLOW_AUDIO_LIBS ${MARSHMALLOW_AUDIO_LIBS}
	                           marshmallow_audio_dummy
	)
endif()

################################################################################

add_library(marshmallow_audio ${MARSHMALLOW_AUDIO_SRCS})

target_link_libraries(marshmallow_audio ${MARSHMALLOW_AUDIO_LIBS})

install(TARGETS marshmallow_audio
        LIBRARY DESTINATION lib COMPONENT runtime
        ARCHIVE DESTINATION lib COMPONENT development
)

