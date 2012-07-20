###############################################################################
# Find BCM_HOST (Broadcom VideoCore)
###############################################################################
#
#  BCM_HOST_FOUND
#  BCM_HOST_INCLUDE_DIRS
#  BCM_HOST_LIBRARIES
#
###############################################################################

find_path(BCM_HOST_INCLUDE_DIR bcm_host.h
	HINTS $ENV{BCM_HOSTDIR}
	PATH_SUFFIXES include
	PATHS /usr/local
	      /usr
	      /opt/vc
	      /opt/local
	      /opt
)
list(APPEND BCM_HOST_INCLUDE_DIRS ${BCM_HOST_INCLUDE_DIR})

find_path(BCM_HOST_PTHREADS_INCLUDE_DIR vcos_platform_types.h
	HINTS $ENV{BCM_HOSTDIR}
	PATH_SUFFIXES "interface/vcos/pthreads"
	PATHS /usr/local
	      /usr
	      /opt/vc
	      /opt/local
	      /opt
)
list(APPEND BCM_HOST_INCLUDE_DIRS ${BCM_HOST_PTHREADS_INCLUDE_DIR})

find_library(BCM_HOST_LIBRARY bcm_host
	HINTS $ENV{BCM_HOSTDIR}
	PATH_SUFFIXES lib
	PATHS /usr/local
	      /usr
	      /opt/vc
	      /opt/local
	      /opt
)
list(APPEND BCM_HOST_LIBRARIES ${BCM_HOST_LIBRARY})

find_library(BCM_HOST_vcos_LIBRARY vcos
	HINTS $ENV{BCM_HOSTDIR}
	PATH_SUFFIXES lib
	PATHS /usr/local
	      /usr
	      /opt/vc
	      /opt/local
	      /opt
)
list(APPEND BCM_HOST_LIBRARIES ${BCM_HOST_vcos_LIBRARY})

find_library(BCM_HOST_vchiq_arm_LIBRARY vchiq_arm
	HINTS $ENV{BCM_HOSTDIR}
	PATH_SUFFIXES lib
	PATHS /usr/local
	      /usr
	      /opt/vc
	      /opt/local
	      /opt
)
list(APPEND BCM_HOST_LIBRARIES ${BCM_HOST_vchiq_arm_LIBRARY})


include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(BCM_HOST DEFAULT_MSG
	BCM_HOST_INCLUDE_DIR
	BCM_HOST_LIBRARY
	BCM_HOST_PTHREADS_INCLUDE_DIR
	BCM_HOST_vchiq_arm_LIBRARY
	BCM_HOST_vcos_LIBRARY
)
MARK_AS_ADVANCED(
	BCM_HOST_INCLUDE_DIR
	BCM_HOST_INCLUDE_DIRS
	BCM_HOST_LIBRARIES
	BCM_HOST_LIBRARY
	BCM_HOST_PTHREADS_INCLUDE_DIR
	BCM_HOST_vchiq_arm_LIBRARY
	BCM_HOST_vcos_LIBRARY)
