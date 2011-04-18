if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_BUILD_TYPE_SHARED_LIBS ON)

find_package(Zlib REQUIRED)
find_package(PNG REQUIRED)

add_subdirectory(src)
