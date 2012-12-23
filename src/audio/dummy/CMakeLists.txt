################################################################################

set(MARSHMALLOW_AUDIO_DUMMY_SRCS backend.cpp
                                 track.cpp)

################################################################################

add_library(marshmallow_audio_dummy ${MARSHMALLOW_AUDIO_DUMMY_SRCS})

