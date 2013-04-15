message(WARNING "You currently only have MARSHMALLOW_GRAPHICS_DUMMY enabled!\n"
                "Please don't forget to select your TARGET VIEWPORT.")

if(MARSHMALLOW_GRAPHICS_DUMMY)
	option(MARSHMALLOW_DUMMY_FAIL "Make dummy viewport fail initialization" OFF)
endif()

set(MARSHMALLOW_GRAPHICS_BACKEND_SRCS backend.cpp
                                      factory.cpp
                                      painter.cpp
                                      texturecoordinatedata.cpp
                                      texturedata.cpp
                                      vertexdata.cpp)

set(MARSHMALLOW_GRAPHICS_BACKEND_LIBS marshmallow_core
	                              marshmallow_event
	                              marshmallow_input
	                              marshmallow_math)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUXX)
	if(BUILD_SHARED_LIBS)
		set(CMAKE_CXX_FLAGS "-fPIC -DPIC ${CMAKE_CXX_FLAGS}")
		set(CMAKE_C_FLAGS "-fPIC -DPIC ${CMAKE_C_FLAGS}")
	endif()
endif()

add_library(marshmallow_graphics_backend STATIC ${MARSHMALLOW_GRAPHICS_BACKEND_SRCS})

target_link_libraries(marshmallow_graphics_backend ${MARSHMALLOW_GRAPHICS_BACKEND_LIBS})

