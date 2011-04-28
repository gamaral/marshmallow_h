add_definitions(-DMATH_LIBRARY)

include_directories(${BOX2D_INCLUDE_DIR})

file(GLOB MARSHMALLOW_MATH_SRCS *.cpp)

add_library(marshmallow_math
	${MARSHMALLOW_MATH_SRCS}
)

target_link_libraries(marshmallow_math
	marshmallow_core
	${BOX2D_LIBRARIES}
)

