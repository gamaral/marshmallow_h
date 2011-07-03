file(GLOB BOX2D_Collision_SRCS Collision/*.cpp)
file(GLOB BOX2D_Shapes_SRCS Collision/Shapes/*.cpp)
file(GLOB BOX2D_Common_SRCS Common/*.cpp)
file(GLOB BOX2D_Dynamics_SRCS Dynamics/*.cpp)
file(GLOB BOX2D_Contacts_SRCS Dynamics/Contacts/*.cpp)
file(GLOB BOX2D_Joints_SRCS Dynamics/Joints/*.cpp)
file(GLOB BOX2D_Rope_SRCS Rope/*.cpp)

set(BOX2D_General_HDRS
	Box2D.h
)

include_directories( ../ )

add_library(marshmallow_box2d STATIC
	${BOX2D_Joints_SRCS}
	${BOX2D_Contacts_SRCS}
	${BOX2D_Dynamics_SRCS}
	${BOX2D_Common_SRCS}
	${BOX2D_Shapes_SRCS}
	${BOX2D_Collision_SRCS}
	${BOX2D_Rope_SRCS}
)

