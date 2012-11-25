if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUXX)
	set(DEBUG_FLAGS "-W -Wall -Wcast-qual -Wconversion -Wextra -Wno-long-long -Wno-variadic-macros -Wshadow -Wwrite-strings -pedantic")
endif()

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${DEBUG_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${DEBUG_FLAGS}")

################################################################## DEFINITIONS #

if(MARSHMALLOW_ANDROID)
	add_definitions(-DMARSHMALLOW_ANDROID)
elseif(MARSHMALLOW_WII)
	add_definitions(-DMARSHMALLOW_WII)
endif()

if(BUILD_SHARED_LIBS)
	add_definitions(-DMARSHMALLOW_SHARED)
endif()

##################################################################### INCLUDES #

include_directories(${PROJECT_BINARY_DIR}/src
                    ${PROJECT_SOURCE_DIR}/src)

############################################################## SUB DIRECTORIES #

add_subdirectory(core)
add_subdirectory(entrypoint)
add_subdirectory(math)
add_subdirectory(event)
add_subdirectory(input)
add_subdirectory(graphics)
add_subdirectory(audio)
add_subdirectory(game)
add_subdirectory(extra)
add_subdirectory(tests)
