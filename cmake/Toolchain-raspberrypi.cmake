# Copyright (c) 2012-2013, Guillermo A. Amaral B. (gamaral) <g@maral.me>
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

cmake_minimum_required(VERSION 2.8)

if(DEFINED CMAKE_CROSSCOMPILING)
	return()
endif()

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)

if ("$ENV{RPI_FIRMWARE_DIR}" STREQUAL ""
    OR "$ENV{RPI_STAGING_DIR}" STREQUAL ""
    OR "$ENV{RPI_SYSTEM_PROCESSOR}" STREQUAL ""
    OR "$ENV{RPI_TOOLCHAIN_DIR}" STREQUAL "")
	message(FATAL_ERROR "\n*** Invalid toolchain environment ***\n"
	                    "Please source the toolchain environment file.\n")
endif()
set(CMAKE_SYSTEM_PROCESSOR $ENV{RPI_SYSTEM_PROCESSOR})

set(RPI_FIRMWARE_DIR  $ENV{RPI_FIRMWARE_DIR}  CACHE INTERNAL "" FORCE)
set(RPI_STAGING_DIR   $ENV{RPI_STAGING_DIR}   CACHE INTERNAL "" FORCE)
set(RPI_TOOLCHAIN_DIR $ENV{RPI_TOOLCHAIN_DIR} CACHE INTERNAL "" FORCE)

set(CMAKE_FIND_ROOT_PATH ${RPI_STAGING_DIR}
                         ${RPI_FIRMWARE_DIR} CACHE INTERNAL "")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(BUILD_UNIT_TESTS OFF CACHE BOOL "")
