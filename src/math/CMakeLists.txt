add_definitions(-DMATH_LIBRARY)

file(GLOB MARSHMALLOW_MATH_SRCS *.cpp)

add_library(marshmallow_math STATIC
	${MARSHMALLOW_MATH_SRCS}
)

target_link_libraries(marshmallow_math marshmallow_core)

