if (BUILD_UNIT_TESTS)
	configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)
	include_directories(${CMAKE_CURRENT_BINARY_DIR})

	set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/tests)

	add_subdirectory(core)
	add_subdirectory(audio)
	add_subdirectory(graphics)
endif (BUILD_UNIT_TESTS)

