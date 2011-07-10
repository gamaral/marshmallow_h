set(BOX2D_BASE "code/Box2D/")

set(BOX2D_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${BOX2D_BASE}/")
set(BOX2D_INCLUDE_DIR "${BOX2D_INCLUDE_DIR}" PARENT_SCOPE)

file(GLOB BOX2D_Collision_SRCS ${BOX2D_BASE}/Box2D/Collision/*.cpp)
file(GLOB BOX2D_Shapes_SRCS ${BOX2D_BASE}/Box2D/Collision/Shapes/*.cpp)
file(GLOB BOX2D_Common_SRCS ${BOX2D_BASE}/Box2D/Common/*.cpp)
file(GLOB BOX2D_Dynamics_SRCS ${BOX2D_BASE}/Box2D/Dynamics/*.cpp)
file(GLOB BOX2D_Contacts_SRCS ${BOX2D_BASE}/Box2D/Dynamics/Contacts/*.cpp)
file(GLOB BOX2D_Joints_SRCS ${BOX2D_BASE}/Box2D/Dynamics/Joints/*.cpp)
file(GLOB BOX2D_Rope_SRCS ${BOX2D_BASE}/Box2D/Rope/*.cpp)

set(BOX2D_General_HDRS
	${BOX2D_BASE}/Box2D/Box2D.h
)

include_directories(${BOX2D_INCLUDE_DIR})

add_library(marshmallow_box2d STATIC
	${BOX2D_Joints_SRCS}
	${BOX2D_Contacts_SRCS}
	${BOX2D_Dynamics_SRCS}
	${BOX2D_Common_SRCS}
	${BOX2D_Shapes_SRCS}
	${BOX2D_Collision_SRCS}
	${BOX2D_Rope_SRCS}
)

