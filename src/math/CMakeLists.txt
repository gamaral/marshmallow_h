add_definitions(-DMARSHMALLOW_MATH_LIBRARY)

file(GLOB MARSHMALLOW_MATH_SRCS "*.cpp")
set(MARSHMALLOW_MATH_LIBS "marshmallow_core")

# box2d
if (MARSHMALLOW_WITH_BOX2D)
	include_directories(${BOX2D_INCLUDE_DIR})
	list(APPEND MARSHMALLOW_MATH_LIBS ${BOX2D_LIBRARY})
endif()

add_library(marshmallow_math ${MARSHMALLOW_MATH_SRCS})

target_link_libraries(marshmallow_math ${MARSHMALLOW_MATH_LIBS})

install(TARGETS marshmallow_math
        LIBRARY DESTINATION lib COMPONENT runtime
        ARCHIVE DESTINATION lib COMPONENT development)

