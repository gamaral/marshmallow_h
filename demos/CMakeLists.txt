include_directories(${PROJECT_BINARY_DIR}/include
                    ${PROJECT_SOURCE_DIR}/include
                    ${BOX2D_INCLUDE_DIR}
                    ${EASTL_INCLUDE_DIR}
                    ${TINYXML_INCLUDE_DIR})

add_subdirectory(engine)
add_subdirectory(marshmallow)
add_subdirectory(angrymallows)

