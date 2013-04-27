find_package(OpenAL)

if(NOT OPENAL_FOUND)
	message(FATAL_ERROR "OpenAL was not found!")
endif()

set(MARSHMALLOW_AUDIO_OPENAL_SRCS "backend.cpp")

set(MARSHMALLOW_AUDIO_OPENAL_LIBS ${OPENAL_LIBRARY})

include_directories(${OPENAL_INCLUDE_DIR})

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUXX)
	if(MARSHMALLOW_SHARED)
		set(CMAKE_CXX_FLAGS "-fPIC -DPIC ${CMAKE_CXX_FLAGS}")
		set(CMAKE_C_FLAGS "-fPIC -DPIC ${CMAKE_C_FLAGS}")
	endif()
endif()

################################################################################

add_library(marshmallow_audio_openal STATIC ${MARSHMALLOW_AUDIO_OPENAL_SRCS})

target_link_libraries(marshmallow_audio_openal ${MARSHMALLOW_AUDIO_OPENAL_LIBS})

