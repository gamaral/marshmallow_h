include_directories(${ZLIB_INCLUDE_DIR}
                    ${PNG_INCLUDE_DIR}
                    ${OPENGL_INCLUDE_DIR}
                    ${CMAKE_SOURCE_DIR}/include/)


set(MARSHMALLOW_GLPNG_SRCS glpng.c)

set(CMAKE_SHARED_LIBRARY_C_FLAGS "-fPIC")
set(CMAKE_STATIC_LIBRARY_C_FLAG "-Wl," )

add_library(marshmallow_glpng ${MARSHMALLOW_BUILD_TYPE} ${MARSHMALLOW_GLPNG_SRCS})

target_link_libraries(marshmallow_glpng ${ZLIB_LIBRARIES}
                                        ${PNG_LIBRARY}
                                        ${OPENGL_LIBRARIES})

