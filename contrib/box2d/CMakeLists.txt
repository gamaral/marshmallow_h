# Copyright 2011 Marshmallow Engine. All rights reserved.
# Copyright 2011 Guillermo A. Amaral B. (gamaral) <g@maral.me>
#
# Redistribution and use in source and binary forms, with or without modification, are
# permitted provided that the following conditions are met:
#
#    1. Redistributions of source code must retain the above copyright notice, this list of
#       conditions and the following disclaimer.
#
#    2. Redistributions in binary form must reproduce the above copyright notice, this list
#       of conditions and the following disclaimer in the documentation and/or other materials
#       provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY MARSHMALLOW ENGINE ``AS IS'' AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
# FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL MARSHMALLOW ENGINE OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# The views and conclusions contained in the software and documentation are those of the
# authors and should not be interpreted as representing official policies, either expressed
# or implied, of Marshmallow Engine.
#

include_directories(${BOX2D_INCLUDE_DIR})

file(GLOB BOX2D_Collision_SRCS ${BOX2D_BASE}/Box2D/Collision/*.cpp)
file(GLOB BOX2D_Shapes_SRCS ${BOX2D_BASE}/Box2D/Collision/Shapes/*.cpp)
file(GLOB BOX2D_Common_SRCS ${BOX2D_BASE}/Box2D/Common/*.cpp)
file(GLOB BOX2D_Dynamics_SRCS ${BOX2D_BASE}/Box2D/Dynamics/*.cpp)
file(GLOB BOX2D_Contacts_SRCS ${BOX2D_BASE}/Box2D/Dynamics/Contacts/*.cpp)
file(GLOB BOX2D_Joints_SRCS ${BOX2D_BASE}/Box2D/Dynamics/Joints/*.cpp)
file(GLOB BOX2D_Rope_SRCS ${BOX2D_BASE}/Box2D/Rope/*.cpp)

if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUXX)
	if (BUILD_SHARED_LIBS)
		set(CMAKE_CXX_FLAGS "-fPIC ${CMAKE_CXX_FLAGS}")
		set(CMAKE_C_FLAGS "-fPIC ${CMAKE_C_FLAGS}")
	endif()
endif()

add_library(${BOX2D_LIBRARY} STATIC
	${BOX2D_Joints_SRCS}
	${BOX2D_Contacts_SRCS}
	${BOX2D_Dynamics_SRCS}
	${BOX2D_Common_SRCS}
	${BOX2D_Shapes_SRCS}
	${BOX2D_Collision_SRCS}
	${BOX2D_Rope_SRCS}
)

install(TARGETS ${BOX2D_LIBRARY}
        LIBRARY DESTINATION lib COMPONENT runtime
        ARCHIVE DESTINATION lib COMPONENT development)

