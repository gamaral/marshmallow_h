if (BUILD_UNIT_TESTS)

	    enable_testing()
	    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/tests)
	    add_subdirectory(core)

endif (BUILD_UNIT_TESTS)
