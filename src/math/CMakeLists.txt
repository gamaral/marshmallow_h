add_definitions(-DMATH_LIBRARY)

file(GLOB MARSHMALLOW_MATH_SRCS *.cpp)

add_library(marshmallow_math ${MARSHMALLOW_BUILD_TYPE}
	${MARSHMALLOW_MATH_SRCS}
)

target_link_libraries(marshmallow_math marshmallow_core)

