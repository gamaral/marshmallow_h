add_subdirectory(engine)
add_subdirectory(tilemap)
add_subdirectory(platformer)

if (MARSHMALLOW_WITH_BOX2D)
	add_subdirectory(angrymallows)
endif()

