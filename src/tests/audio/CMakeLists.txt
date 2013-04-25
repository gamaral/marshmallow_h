file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/data"
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(MASHMALLOW_TEST_AUDIO_LIBS "marshmallow_core"
                               "marshmallow_audio"
)

add_executable(test_audio_tracks ${TEST_MAIN} "tracks.cpp")

target_link_libraries(test_audio_tracks ${MASHMALLOW_TEST_AUDIO_LIBS})

add_test(NAME audio_tracks COMMAND test_audio_tracks)

