include(Box2D)

message(STATUS ${BOX2D_BASE}/Box2D/Collision/)
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

add_library(${BOX2D_LIBRARY} STATIC
	${BOX2D_Joints_SRCS}
	${BOX2D_Contacts_SRCS}
	${BOX2D_Dynamics_SRCS}
	${BOX2D_Common_SRCS}
	${BOX2D_Shapes_SRCS}
	${BOX2D_Collision_SRCS}
	${BOX2D_Rope_SRCS}
)

