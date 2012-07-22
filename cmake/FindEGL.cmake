###############################################################################
# Find EGL
###############################################################################
#
#  EGL_FOUND
#  EGL_INCLUDE_DIR
#  EGL_LIBRARY
#
###############################################################################

find_path(EGL_INCLUDE_DIR EGL/egl.h
	HINTS $ENV{EGLDIR}
	PATH_SUFFIXES include
	PATHS ~/Library/Frameworks
	      /Library/Frameworks
	      /usr/local/include/EGL
	      /usr/local/include
	      /usr/include
	      /opt/local
	      /opt
)

find_library(EGL_LIBRARY
	EGL
	HINTS $ENV{EGLDIR}
	PATH_SUFFIXES lib64 lib
	PATHS ~/Library/Frameworks
	      /Library/Frameworks
	      /usr/local
	      /usr
	      /usr/X11R6
	      /opt/local
	      /opt
)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(EGL DEFAULT_MSG EGL_LIBRARY)

mark_as_advanced(EGL_LIBRARY EGL_INCLUDE_DIR)
