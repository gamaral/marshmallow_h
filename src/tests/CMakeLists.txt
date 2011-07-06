if (BUILD_UNIT_TESTS)
	set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/tests)
	add_subdirectory(core)
endif (BUILD_UNIT_TESTS)
