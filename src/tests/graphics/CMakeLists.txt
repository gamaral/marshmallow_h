
SET(MASHMALLOW_TEST_GRAPHICS_LIBS marshmallow_entrypoint marshmallow_core marshmallow_graphics)

# TINYXML
include_directories(${TINYXML_INCLUDE_DIR})
set(MARSHMALLOW_GRAPHICS_LIBS ${MARSHMALLOW_GRAPHICS_LIBS} ${TINYXML_LIBRARIES})

add_executable(marshmallow_test_graphics_tileset tileset.cpp)

target_link_libraries(marshmallow_test_graphics_tileset ${MASHMALLOW_TEST_GRAPHICS_LIBS})

add_test(NAME marshmallow_test_graphics_tileset COMMAND marshmallow_test_graphics_tileset)

