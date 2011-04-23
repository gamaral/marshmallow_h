if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUXX)
	set(CMAKE_C_FLAGS_DEBUG "-O0 -fno-exceptions -Wextra -Wall -W -pedantic -Wshadow -Wcast-qual -Wwrite-strings -Wconversion -Wno-variadic-macros -Wno-long-long -ansi ${CMAKE_C_FLAGS_DEBUG}")
	set(CMAKE_CXX_FLAGS_DEBUG "-O0 -fno-exceptions -Wextra -Wall -W -pedantic -Wshadow -Wcast-qual -Wwrite-strings -Wconversion -Wno-variadic-macros -Wno-long-long -std=c++98 ${CMAKE_CXX_FLAGS_DEBUG}")
	set(CMAKE_CXX_FLAGS_DEBUG "-Weffc++ -Wold-style-cast ${CMAKE_CXX_FLAGS_DEBUG}")
endif()

add_subdirectory(core)
add_subdirectory(math)
add_subdirectory(event)
add_subdirectory(graphics)
add_subdirectory(game)

