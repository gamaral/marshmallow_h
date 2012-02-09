file(GLOB ANGRYMALLOWS_DEMO_SRCS *.cpp)

include(Box2D)

add_executable(angrymallows_demo ${ANGRYMALLOWS_DEMO_SRCS})

if (APPLE)
	# Xcode workaround
	set_target_properties(angrymallows_demo PROPERTIES
	    COMPILE_FLAGS -DDEMO_CWD=\\\\"${CMAKE_CURRENT_SOURCE_DIR}\\\\")
else ()
	set_target_properties(angrymallows_demo PROPERTIES
	    COMPILE_FLAGS -DDEMO_CWD=\\"${CMAKE_CURRENT_SOURCE_DIR}\\")
endif()

if(WIN32 AND NOT MARSHMALLOW_DEBUG)
	set_target_properties(angrymallows_demo PROPERTIES WIN32_EXECUTABLE ON)
endif()

target_link_libraries(angrymallows_demo marshmallow_entrypoint
                                        marshmallow_game)

