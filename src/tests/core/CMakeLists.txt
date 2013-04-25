file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/data"
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(MASHMALLOW_TEST_CORE_LIBS "marshmallow_core")

add_executable(test_core_hash ${TEST_MAIN} "hash.cpp")
add_executable(test_core_shared ${TEST_MAIN} "shared.cpp")
add_executable(test_core_base64 ${TEST_MAIN} "base64.cpp")
add_executable(test_core_fileio ${TEST_MAIN} "fileio.cpp")
add_executable(test_core_bufferio ${TEST_MAIN} "bufferio.cpp")

target_link_libraries(test_core_hash ${MASHMALLOW_TEST_CORE_LIBS})
target_link_libraries(test_core_shared ${MASHMALLOW_TEST_CORE_LIBS})
target_link_libraries(test_core_base64 ${MASHMALLOW_TEST_CORE_LIBS})
target_link_libraries(test_core_fileio ${MASHMALLOW_TEST_CORE_LIBS})
target_link_libraries(test_core_bufferio ${MASHMALLOW_TEST_CORE_LIBS})

add_test(NAME core_hash     COMMAND test_core_hash)
add_test(NAME core_shared   COMMAND test_core_shared)
add_test(NAME core_base64   COMMAND test_core_base64)
add_test(NAME core_fileio   COMMAND test_core_fileio)
add_test(NAME core_bufferio COMMAND test_core_bufferio)

