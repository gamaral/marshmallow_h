################################################################################

set(MARSHMALLOW_AUDIO_ALSA_SRCS backend.cpp)

find_package(ALSA)
set(MARSHMALLOW_AUDIO_ALSA_LIBS ${ALSA_LIBRARY})

include_directories(${ALSA_INCLUDE_DIR})

################################################################################

add_library(marshmallow_audio_alsa ${MARSHMALLOW_AUDIO_ALSA_SRCS})

target_link_libraries(marshmallow_audio_alsa ${MARSHMALLOW_AUDIO_ALSA_LIBS})

