set(MARSHMALLOW_BUILD_TITLE "Marshmallow HARMATTAN_ARMEL" CACHE STRING "Build Title")

option(MARSHMALLOW_DEBUG "Marshmallow Debugging" ON)
set(MARSHMALLOW_DEBUG_VERBOSITY "0" CACHE STRING "Verbosity Level")

set(MARSHMALLOW_VIEWPORT_DEPTH   "16"  CACHE STRING "Viewport default depth")
set(MARSHMALLOW_VIEWPORT_REFRESH "60"  CACHE STRING "Viewport default refresh rate")

option(MARSHMALLOW_VIEWPORT_FULLSCREEN "Viewport default fullscreen mode" ON)
option(MARSHMALLOW_VIEWPORT_GRAB_INPUT "Takeover viewport input devices"  ON)
option(MARSHMALLOW_VIEWPORT_LOCK_WIDTH "Use width for aspect ratio resolucion" OFF)
option(MARSHMALLOW_VIEWPORT_VSYNC      "Viewport default vsync mode"      OFF)

option(MARSHMALLOW_VIEWPORT_OPENGL "Use OpenGL Viewport"                 ON)
option(MARSHMALLOW_OPENGL_GLX "Use OpenGL GLX Viewport (unix)"           ON)
option(MARSHMALLOW_OPENGL_GLES2 "Use EGL + OpenGL ES 2.0 (requires GLX)" ON)

option(MARSHMALLOW_WITH_BOX2D "Use Box2D" ON)
option(MARSHMALLOW_WITH_LUA   "Use Lua"   OFF)
