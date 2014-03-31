# Copyright (c) 2012, Guillermo A. Amaral B. (gamaral) <g@maral.me>.
# All rights reserved.
#
# This file is part of Marshmallow Game Engine.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#    1. Redistributions of source code must retain the above copyright notice,
#       this list of conditions and the following disclaimer.
#
#    2. Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
# EVENT SHALL THE CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# The views and conclusions contained in the software and documentation are
# those of the authors and should not be interpreted as representing official
# policies, either expressed or implied, of the project as a whole.
#
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

find_path(BCM_HOST_CONFIG_INCLUDE_DIR vchost_config.h
	HINTS $ENV{BCM_HOSTDIR}
	PATH_SUFFIXES include/interface/vmcs_host/linux
	PATHS /usr/local
	      /usr
	      /opt/vc
	      /opt/local
	      /opt
)
list(APPEND BCM_HOST_INCLUDE_DIRS ${BCM_HOST_CONFIG_INCLUDE_DIR})

find_path(BCM_HOST_PTHREADS_INCLUDE_DIR vcos_platform_types.h
	HINTS $ENV{BCM_HOSTDIR}
	PATH_SUFFIXES include/interface/vcos/pthreads
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

find_library(BCM_HOST_khrn_client_LIBRARY khrn_client
	HINTS $ENV{BCM_HOSTDIR}
	PATH_SUFFIXES lib
	PATHS /usr/local
	      /usr
	      /opt/vc
	      /opt/local
	      /opt
)
list(APPEND BCM_HOST_LIBRARIES ${BCM_HOST_khrn_client_LIBRARY})

find_library(BCM_HOST_vcfiled_check_LIBRARY vcfiled_check
	HINTS $ENV{BCM_HOSTDIR}
	PATH_SUFFIXES lib
	PATHS /usr/local
	      /usr
	      /opt/vc
	      /opt/local
	      /opt
)
list(APPEND BCM_HOST_LIBRARIES ${BCM_HOST_vcfiled_check_LIBRARY})

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

find_library(BCM_HOST_vchostif_LIBRARY vchostif
	HINTS $ENV{BCM_HOSTDIR}
	PATH_SUFFIXES lib
	PATHS /usr/local
	      /usr
	      /opt/vc
	      /opt/local
	      /opt
)
list(APPEND BCM_HOST_LIBRARIES ${BCM_HOST_vchostif_LIBRARY})

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

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(BCM_HOST DEFAULT_MSG
	BCM_HOST_INCLUDE_DIR
	BCM_HOST_LIBRARY
	BCM_HOST_PTHREADS_INCLUDE_DIR
	BCM_HOST_khrn_client_LIBRARY
	BCM_HOST_vcfiled_check_LIBRARY
	BCM_HOST_vchiq_arm_LIBRARY
	BCM_HOST_vchostif_LIBRARY
	BCM_HOST_vcos_LIBRARY
)

mark_as_advanced(
	BCM_HOST_INCLUDE_DIR
	BCM_HOST_INCLUDE_DIRS
	BCM_HOST_LIBRARIES
	BCM_HOST_LIBRARY
	BCM_HOST_PTHREADS_INCLUDE_DIR
	BCM_HOST_khrn_client_LIBRARY
	BCM_HOST_vcfiled_check_LIBRARY
	BCM_HOST_vchiq_arm_LIBRARY
	BCM_HOST_vchostif_LIBRARY
	BCM_HOST_vcos_LIBRARY
)
