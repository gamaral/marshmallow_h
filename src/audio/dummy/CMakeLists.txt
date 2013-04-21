set(MARSHMALLOW_AUDIO_DUMMY_SRCS "backend.cpp")

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUXX)
	if(MARSHMALLOW_SHARED)
		set(CMAKE_CXX_FLAGS "-fPIC -DPIC ${CMAKE_CXX_FLAGS}")
		set(CMAKE_C_FLAGS "-fPIC -DPIC ${CMAKE_C_FLAGS}")
	endif()
endif()

################################################################################

add_library(marshmallow_audio_backend STATIC ${MARSHMALLOW_AUDIO_DUMMY_SRCS})

