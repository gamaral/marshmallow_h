###################################################################### OPTIONS #

option(MARSHMALLOW_DUMMY_FAIL "Make dummy viewport fail initialization" OFF)

################################################################################

add_definitions(-DMARSHMALLOW_GRAPHICS_DUMMY)

if(MARSHMALLOW_DUMMY_FAIL)
	add_definitions(-DMARSHMALLOW_DUMMY_FAIL)
endif()

message(WARNING "You currently only have MARSHMALLOW_GRAPHICS_DUMMY enabled!\n"
                "Please don't forget to select a proper graphics backend.")

set(MARSHMALLOW_GRAPHICS_BACKEND_SRCS "backend.cpp"
                                      "factory.cpp"
                                      "painter.cpp"
                                      "texturecoordinatedata.cpp"
                                      "texturedata.cpp"
                                      "vertexdata.cpp"
)

set(MARSHMALLOW_GRAPHICS_BACKEND_LIBS "marshmallow_core"
	                              "marshmallow_event"
	                              "marshmallow_input"
	                              "marshmallow_math"
)

if(MARSHMALLOW_SHARED)
	# marshmallow_graphics is shared
	if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
		set(CMAKE_CXX_FLAGS "-fPIC -DPIC ${CMAKE_CXX_FLAGS}")
		set(CMAKE_C_FLAGS "-fPIC -DPIC ${CMAKE_C_FLAGS}")
	endif()
else() # marshmallow_graphics is static
	list(APPEND MARSHMALLOW_GRAPHICS_BACKEND_LIBS "marshmallow_graphics")
endif()

add_library(marshmallow_graphics_backend STATIC ${MARSHMALLOW_GRAPHICS_BACKEND_SRCS})

target_link_libraries(marshmallow_graphics_backend ${MARSHMALLOW_GRAPHICS_BACKEND_LIBS})

