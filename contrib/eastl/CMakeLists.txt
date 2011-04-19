set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB MARSHMALLOW_EASTL_SRCS src/*.cpp)

if(BUILD_SHARED_LIBS)
	add_definitions(-DEASTL_DLL)
endif(BUILD_SHARED_LIBS)


add_library(marshmallow_eastl
	${MARSHMALLOW_EASTL_SRCS}
)

