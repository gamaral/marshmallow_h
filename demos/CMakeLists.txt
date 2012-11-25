set(MARSHMALLOW_DEMO_CWD_OVERRIDE ON CACHE BOOL "Override Current Working Directory in demos")

if(MARSHMALLOW_ANDROID)
	set(MARSHMALLOW_DEMO_LIBRARIES TRUE)
endif()
set(MARSHMALLOW_DEMO_CWD_OVERRIDE ON CACHE BOOL "Override Current Working Directory in demos")

add_subdirectory(engine)
add_subdirectory(tilemap)
add_subdirectory(platformer)
if(MARSHMALLOW_WITH_BOX2D)
	add_subdirectory(angrymallows)
endif()

