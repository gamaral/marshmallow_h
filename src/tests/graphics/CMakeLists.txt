set(MASHMALLOW_TEST_GRAPHICS_LIBS "marshmallow_core"
                                  "marshmallow_graphics"
)

add_executable(test_graphics_tileset "tileset.cpp")

target_link_libraries(test_graphics_tileset ${MASHMALLOW_TEST_GRAPHICS_LIBS})

add_test(NAME graphics_tileset COMMAND test_graphics_tileset)

