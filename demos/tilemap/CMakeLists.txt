if(MARSHMALLOW_DEMO_LIBRARIES)
	add_library(tilemap_demo0 SHARED "demo0.cpp")
	add_library(tilemap_demo1 SHARED "demo1.cpp"
	                                 "colliderentity.cpp"
	                                 "customfactory.cpp"
	                                 "inputcomponent.cpp"
	                                 "playerentity.cpp"
	)
	add_library(tilemap_demo2 SHARED "demo2.cpp")
else()
	add_executable(tilemap_demo0 "demo0.cpp")
	add_executable(tilemap_demo1 "demo1.cpp"
	                             "colliderentity.cpp"
	                             "customfactory.cpp"
	                             "inputcomponent.cpp"
	                             "playerentity.cpp"
	)
	add_executable(tilemap_demo2 "demo2.cpp")
endif()

if (MARSHMALLOW_DEMO_CWD_OVERRIDE)
	if (APPLE)
		# Xcode workaround
		set_target_properties(tilemap_demo0 PROPERTIES
		    COMPILE_FLAGS -DDEMO_CWD=\\\\"${CMAKE_CURRENT_SOURCE_DIR}\\\\")
		set_target_properties(tilemap_demo1 PROPERTIES
		    COMPILE_FLAGS -DDEMO_CWD=\\\\"${CMAKE_CURRENT_SOURCE_DIR}\\\\")
		set_target_properties(tilemap_demo2 PROPERTIES
		    COMPILE_FLAGS -DDEMO_CWD=\\\\"${CMAKE_CURRENT_SOURCE_DIR}\\\\")
	else()
		set_target_properties(tilemap_demo0 PROPERTIES
		    COMPILE_FLAGS -DDEMO_CWD=\\"${CMAKE_CURRENT_SOURCE_DIR}\\")
		set_target_properties(tilemap_demo1 PROPERTIES
		    COMPILE_FLAGS -DDEMO_CWD=\\"${CMAKE_CURRENT_SOURCE_DIR}\\")
		set_target_properties(tilemap_demo2 PROPERTIES
		    COMPILE_FLAGS -DDEMO_CWD=\\"${CMAKE_CURRENT_SOURCE_DIR}\\")
	endif()
endif()

if(WIN32 AND NOT MARSHMALLOW_DEBUG)
	set_target_properties(tilemap_demo0 PROPERTIES WIN32_EXECUTABLE ON)
	set_target_properties(tilemap_demo1 PROPERTIES WIN32_EXECUTABLE ON)
	set_target_properties(tilemap_demo2 PROPERTIES WIN32_EXECUTABLE ON)
endif()

target_link_libraries(tilemap_demo0 marshmallow_entrypoint
                                    marshmallow_game
)

target_link_libraries(tilemap_demo1 marshmallow_entrypoint
                                    marshmallow_game
                                    marshmallow_extra
)

target_link_libraries(tilemap_demo2 marshmallow_entrypoint
                                    marshmallow_game
)

install(TARGETS tilemap_demo0 tilemap_demo1 tilemap_demo2
        RUNTIME DESTINATION bin COMPONENT demos
        LIBRARY DESTINATION bin COMPONENT demos)

install(DIRECTORY assets/
        DESTINATION assets COMPONENT demos)

