include_directories(${PROJECT_BINARY_DIR}/src
                    ${PROJECT_SOURCE_DIR}/src
                    ${BOX2D_INCLUDE_DIR}
                    ${EASTL_INCLUDE_DIR}
                    ${TINYXML_INCLUDE_DIR})

add_subdirectory(engine)
add_subdirectory(marshmallow)
add_subdirectory(angrymallows)

