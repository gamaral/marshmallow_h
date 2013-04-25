set(MASHMALLOW_TEST_AUDIO_LIBS "marshmallow_core"
                               "marshmallow_audio"
)

add_executable(test_audio_codecs ${TEST_MAIN} "codecs.cpp")

target_link_libraries(test_audio_codecs ${MASHMALLOW_TEST_AUDIO_LIBS})

add_test(NAME audio_codecs COMMAND test_audio_codecs)

