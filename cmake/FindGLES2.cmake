###############################################################################
# Find GLES2
###############################################################################
#
#  GLES2_FOUND
#  GLES2_INCLUDE_DIR
#  GLES2_LIBRARY
#
###############################################################################

find_path(GLES2_INCLUDE_DIR GLES2/gl2.h
	HINTS $ENV{GLES2DIR}
	PATH_SUFFIXES include
	PATHS ~/Library/Frameworks
	      /Library/Frameworks
	      /usr/local
	      /usr
	      /usr/X11R6
	      /opt/local
	      /opt/vc
	      /opt
)

find_library(GLES2_LIBRARY
	GLESv2
	HINTS $ENV{GLES2DIR}
	PATH_SUFFIXES lib64 lib
	PATHS ~/Library/Frameworks
	      /Library/Frameworks
	      /usr/local
	      /usr
	      /usr/X11R6
	      /opt/local
	      /opt/vc
	      /opt
)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLES2 DEFAULT_MSG GLES2_LIBRARY)

mark_as_advanced(GLES2_LIBRARY GLES2_INCLUDE_DIR)

set(OPENGL_FOUND ${GLES2_FOUND})
