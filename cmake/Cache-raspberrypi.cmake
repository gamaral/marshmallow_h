set(VC_PREFIX "/opt/vc")

set(BUILD_SHARED_LIBS ON CACHE BOOL "")

set(MARSHMALLOW_BUILD_TITLE "Marshmallow Raspberry Pi" CACHE STRING "Build Title")

set(MARSHMALLOW_VIEWPORT_HEIGHT  "480"    CACHE STRING "Viewport window height")
set(MARSHMALLOW_VIEWPORT_WIDTH   "720"    CACHE STRING "Viewport window width")
set(MARSHMALLOW_VIEWPORT_DEPTH   "32"     CACHE STRING "Viewport default depth")

option(MARSHMALLOW_VIEWPORT_OPENGL "Use OpenGL Viewport"                 ON)
option(MARSHMALLOW_OPENGL_VC  "Use OpenGL VideoCore Viewport (broadcom)" ON)
option(MARSHMALLOW_OPENGL_GLES2 "Use EGL + OpenGL ES 2.0 (requires VC)"  ON)

option(MARSHMALLOW_CONTRIB_LIBPNG "Use bundled LibPNG" ON)
option(MARSHMALLOW_CONTRIB_LUA    "Use bundled LUA"    ON)
option(MARSHMALLOW_CONTRIB_ZLIB   "Use bundled ZLIB"   ON)

option(MARSHMALLOW_WITH_BOX2D "Use Box2D" ON)
option(MARSHMALLOW_WITH_LUA   "Use Lua"   OFF)

set(EGL_INCLUDE_DIR   "${VC_PREFIX}/include/" CACHE STRING "")
set(EGL_LIBRARY       "${VC_PREFIX}/lib/libEGL.so" CACHE STRING "")

set(GLES2_INCLUDE_DIR "${VC_PREFIX}/include/" CACHE STRING "")
set(GLES2_LIBRARY     "${VC_PREFIX}/lib/libGLESv2.so" CACHE STRING "")
