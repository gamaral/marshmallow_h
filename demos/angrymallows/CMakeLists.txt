file(GLOB DEMO_SRCS *.cpp)

if(MARSHMALLOW_DEMO_LIBRARIES)
	add_library(angrymallows_demo SHARED ${DEMO_SRCS})
else()
	add_executable(angrymallows_demo ${DEMO_SRCS})
endif()

# BOX2D
include_directories(${BOX2D_INCLUDE_DIR})

if (MARSHMALLOW_DEMO_CWD_OVERRIDE)
	if (APPLE)
		# Xcode workaround
		set_target_properties(angrymallows_demo PROPERTIES
		    COMPILE_FLAGS -DDEMO_CWD=\\\\"${CMAKE_CURRENT_SOURCE_DIR}\\\\")
	else ()
		set_target_properties(angrymallows_demo PROPERTIES
		    COMPILE_FLAGS -DDEMO_CWD=\\"${CMAKE_CURRENT_SOURCE_DIR}\\")
	endif()
endif()

if(WIN32 AND NOT MARSHMALLOW_DEBUG)
	set_target_properties(angrymallows_demo PROPERTIES WIN32_EXECUTABLE ON)
endif()

target_link_libraries(angrymallows_demo marshmallow_entrypoint
                                        marshmallow_game
                                        ${BOX2D_LIBRARIES})

install(TARGETS angrymallows_demo
        RUNTIME DESTINATION bin COMPONENT demos
        LIBRARY DESTINATION bin COMPONENT demos)

install(DIRECTORY assets/
        DESTINATION assets COMPONENT demos)

