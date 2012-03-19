add_definitions(-DMARSHMALLOW_MATH_LIBRARY)

file(GLOB MARSHMALLOW_MATH_SRCS *.cpp)
set(MARSHMALLOW_MATH_LIBS marshmallow_core)

# BOX2D
if (MARSHMALLOW_WITH_BOX2D)
	include_directories(${BOX2D_INCLUDE_DIR})
	set(MARSHMALLOW_MATH_LIBS ${MARSHMALLOW_MATH_LIBS} ${BOX2D_LIBRARIES})
endif()

add_library(marshmallow_math ${MARSHMALLOW_MATH_SRCS})

target_link_libraries(marshmallow_math ${MARSHMALLOW_MATH_LIBS})

