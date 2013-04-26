###################################################################### OPTIONS #

option(MARSHMALLOW_AUDIO_DUMMY  "Use Dummy Audio Backend"  ON)
option(MARSHMALLOW_AUDIO_OPENAL "Use OpenAL Audio Backend" OFF)

if(UNIX AND ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	option(MARSHMALLOW_AUDIO_ALSA   "Use ALSA Audio Backend"   OFF)
endif()

################################################################################

add_definitions(-DMARSHMALLOW_AUDIO_LIBRARY)

set(MARSHMALLOW_AUDIO_SRCS "pcm.cpp"
                           "player.cpp"
                           "oggtrack.cpp"
                           "wavetrack.cpp"
)

set(MARSHMALLOW_AUDIO_LIBS "marshmallow_core")

include_directories(${OGG_INCLUDE_DIR})
list(APPEND MARSHMALLOW_AUDIO_LIBS ${OGG_LIBRARY})

include_directories(${VORBIS_INCLUDE_DIRS})
list(APPEND MARSHMALLOW_AUDIO_LIBS ${VORBIS_LIBRARIES})

if(MARSHMALLOW_AUDIO_ALSA)
	add_subdirectory(alsa)
	list(APPEND MARSHMALLOW_AUDIO_LIBS "marshmallow_audio_alsa")
elseif(MARSHMALLOW_AUDIO_OPENAL)
	add_subdirectory(openal)
	list(APPEND MARSHMALLOW_AUDIO_LIBS "marshmallow_audio_openal")
elseif(MARSHMALLOW_AUDIO_DUMMY)
	add_subdirectory(dummy)
	list(APPEND MARSHMALLOW_AUDIO_LIBS "marshmallow_audio_dummy")
endif()

################################################################################

add_library(marshmallow_audio ${MARSHMALLOW_AUDIO_SRCS})

target_link_libraries(marshmallow_audio ${MARSHMALLOW_AUDIO_LIBS})

install(TARGETS marshmallow_audio
        LIBRARY DESTINATION lib COMPONENT runtime
        ARCHIVE DESTINATION lib COMPONENT development
)

