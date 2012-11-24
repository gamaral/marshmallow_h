add_definitions(-DMARSHMALLOW_AUDIO_LIBRARY)

file(GLOB MARSHMALLOW_AUDIO_SRCS *.cpp)
set(MARSHMALLOW_EVENT_LIBS marshmallow_core)

add_library(marshmallow_audio ${MARSHMALLOW_AUDIO_SRCS})

target_link_libraries(marshmallow_event ${MARSHMALLOW_AUDIO_LIBS})

install(TARGETS marshmallow_audio
        LIBRARY DESTINATION lib COMPONENT runtime
        ARCHIVE DESTINATION lib COMPONENT development)

