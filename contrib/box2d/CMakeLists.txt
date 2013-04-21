project(marshmallow_box2d)

include_directories(${BOX2D_INCLUDE_DIR})

set(BOX2D_BASE "${CMAKE_CURRENT_SOURCE_DIR}/code/Box2D")

file(GLOB BOX2D_Collision_SRCS ${BOX2D_BASE}/Box2D/Collision/*.cpp)
file(GLOB BOX2D_Shapes_SRCS ${BOX2D_BASE}/Box2D/Collision/Shapes/*.cpp)
file(GLOB BOX2D_Common_SRCS ${BOX2D_BASE}/Box2D/Common/*.cpp)
file(GLOB BOX2D_Dynamics_SRCS ${BOX2D_BASE}/Box2D/Dynamics/*.cpp)
file(GLOB BOX2D_Contacts_SRCS ${BOX2D_BASE}/Box2D/Dynamics/Contacts/*.cpp)
file(GLOB BOX2D_Joints_SRCS ${BOX2D_BASE}/Box2D/Dynamics/Joints/*.cpp)
file(GLOB BOX2D_Rope_SRCS ${BOX2D_BASE}/Box2D/Rope/*.cpp)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUXX)
	if(BUILD_SHARED_LIBS)
		set(CMAKE_CXX_FLAGS "-fPIC -DPIC ${CMAKE_CXX_FLAGS}")
		set(CMAKE_C_FLAGS "-fPIC -DPIC ${CMAKE_C_FLAGS}")
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

