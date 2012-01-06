include(Box2D)

add_executable(platformer_demo0 "demo0.cpp"
                                "colliderentity.cpp"
                                "customfactory.cpp"
                                "inputcomponent.cpp"
                                "playercollidercomponent.cpp"
                                "playerentity.cpp"
)

if (APPLE)
	# Xcode workaround
	set_target_properties(platformer_demo0 PROPERTIES
	    COMPILE_FLAGS -DDEMO_CWD=\\\\"${CMAKE_CURRENT_SOURCE_DIR}\\\\")
else()
	set_target_properties(platformer_demo0 PROPERTIES
	    COMPILE_FLAGS -DDEMO_CWD=\\"${CMAKE_CURRENT_SOURCE_DIR}\\")
endif()

if(WIN32 AND NOT MARSHMALLOW_DEBUG)
	set_target_properties(platformer_demo0 PROPERTIES WIN32_EXECUTABLE ON)
endif()

target_link_libraries(platformer_demo0 marshmallow_entrypoint
                                       marshmallow_game
                                       marshmallow_extra
)

