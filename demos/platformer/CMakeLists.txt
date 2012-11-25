set(DEMO_SRCS "demo0.cpp"
              "../common/actorentity.cpp"
              "../common/colliderentity.cpp"
              "../common/deathevent.cpp"
              "../common/doomevent.cpp"
              "../common/warpevent.cpp"
              "actorcollidercomponent.cpp"
              "customfactory.cpp"
              "inputcomponent.cpp"
              "playerentity.cpp"
)

if(MARSHMALLOW_DEMO_LIBRARIES)
	add_library(platformer_demo0 SHARED ${DEMO_SRCS})
else()
	add_executable(platformer_demo0 ${DEMO_SRCS})
endif()

if (MARSHMALLOW_DEMO_CWD_OVERRIDE)
	if (APPLE)
		# Xcode workaround
		set_target_properties(platformer_demo0 PROPERTIES
		    COMPILE_FLAGS -DDEMO_CWD=\\\\"${CMAKE_CURRENT_SOURCE_DIR}\\\\")
	else ()
		set_target_properties(platformer_demo0 PROPERTIES
		    COMPILE_FLAGS -DDEMO_CWD=\\"${CMAKE_CURRENT_SOURCE_DIR}\\")
	endif()
endif()

if(WIN32 AND NOT MARSHMALLOW_DEBUG)
	set_target_properties(platformer_demo0 PROPERTIES WIN32_EXECUTABLE ON)
endif()

target_link_libraries(platformer_demo0 marshmallow_entrypoint
                                       marshmallow_game
                                       marshmallow_extra
)

install(TARGETS platformer_demo0
        RUNTIME DESTINATION bin COMPONENT demos
        LIBRARY DESTINATION bin COMPONENT demos)

install(DIRECTORY assets/
        DESTINATION assets COMPONENT demos)

