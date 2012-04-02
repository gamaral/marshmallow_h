if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUXX)
	set(DEBUG_FLAGS "-W -Wall -Wcast-qual -Wconversion -Wextra -Wno-long-long -Wno-variadic-macros -Wshadow -Wwrite-strings -pedantic")
endif()

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${DEBUG_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${DEBUG_FLAGS}")

##################################################################### INCLUDES #

include_directories(${PROJECT_BINARY_DIR}/src
                    ${PROJECT_SOURCE_DIR}/src)

if (WIN32 AND BUILD_SHARED_LIBS)
    add_definitions(-DMARSHMALLOW_DLL)
endif()

############################################################## SUB DIRECTORIES #

add_subdirectory(core)
add_subdirectory(entrypoint)
add_subdirectory(event)
add_subdirectory(extra)
add_subdirectory(game)
add_subdirectory(graphics)
add_subdirectory(math)
add_subdirectory(tests)

