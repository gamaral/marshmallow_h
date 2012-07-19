# MARSHMALLOW

set(MARSHMALLOW_BUILD_TITLE "Marshmallow Raspberry Pi" CACHE STRING "Build Title")

set(MARSHMALLOW_VIEWPORT_HEIGHT "480" CACHE STRING "Viewport window height")
set(MARSHMALLOW_VIEWPORT_WIDTH  "720" CACHE STRING "Viewport window width")
set(MARSHMALLOW_VIEWPORT_DEPTH  "24"  CACHE STRING "Viewport default depth")

option(MARSHMALLOW_VIEWPORT_DUMMY      "Use Dummy Viewport (testing)"            OFF)
option(MARSHMALLOW_VIEWPORT_OPENGL     "Use OpenGL Viewport"                      ON)
option(MARSHMALLOW_VIEWPORT_FULLSCREEN "Viewport default fullscreen mode"         ON)
option(MARSHMALLOW_OPENGL_VC           "Use OpenGL VideoCore Viewport (broadcom)" ON)
option(MARSHMALLOW_OPENGL_GLES2        "Use EGL + OpenGL ES 2.0 (requires VC)"    ON)

option(MARSHMALLOW_CONTRIB_LIBPNG "Use bundled LibPNG" ON)
option(MARSHMALLOW_CONTRIB_LUA    "Use bundled LUA"    ON)
option(MARSHMALLOW_CONTRIB_ZLIB   "Use bundled ZLIB"   ON)

option(MARSHMALLOW_WITH_BOX2D     "Use Box2D"          ON)
option(MARSHMALLOW_WITH_LUA       "Use Lua"           OFF)

# CMAKE

option(BUILD_SHARED_LIBS "Build a shared library" ON)

set(CMAKE_C_FLAGS   "-mcpu=arm1176jzf-s -mfpu=vfp -mfloat-abi=softfp -fomit-frame-pointer" CACHE STRING "Flags used by the compiler")
set(CMAKE_CXX_FLAGS "-mcpu=arm1176jzf-s -mfpu=vfp -mfloat-abi=softfp -fomit-frame-pointer" CACHE STRING "Flags used by the compiler")

set(CMAKE_FIND_ROOT_PATH "/opt/vc")

set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "/opt/vc/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_SKIP_BUILD_RPATH FALSE)

# FIXED LOCATIONS

#set(VC_PREFIX "$ENV{RPIFIRMWARE}/opt/vc")
#
#set(EGL_INCLUDE_DIR   "${VC_PREFIX}/include/"         CACHE STRING "EGL include directory")
#set(EGL_LIBRARY       "${VC_PREFIX}/lib/libEGL.so"    CACHE STRING "EGL library path")
#
#set(GLES2_INCLUDE_DIR "${VC_PREFIX}/include/"         CACHE STRING "GLESv2 include directory")
#set(GLES2_LIBRARY     "${VC_PREFIX}/lib/libGLESv2.so" CACHE STRING "GLESv2 library path")

