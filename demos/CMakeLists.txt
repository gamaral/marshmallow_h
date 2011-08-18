include_directories(${PROJECT_BINARY_DIR}/src
                    ${PROJECT_SOURCE_DIR}/src
                    ${EASTL_INCLUDE_DIR}
                    ${TINYXML_INCLUDE_DIR})

add_subdirectory(engine)
add_subdirectory(tilemap)

if (MARSHMALLOW_WITH_BOX2D)
	add_subdirectory(angrymallows)
endif()

