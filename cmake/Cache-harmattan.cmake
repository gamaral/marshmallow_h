# Copyright 2012 Marshmallow Engine. All rights reserved.
# Copyright 2012 Guillermo A. Amaral B. (gamaral) <g@maral.me>.
#
# Redistribution and use in source and binary forms, with or without modification, are
# permitted provided that the following conditions are met:
#
#    1. Redistributions of source code must retain the above copyright notice, this list of
#       conditions and the following disclaimer.
#
#    2. Redistributions in binary form must reproduce the above copyright notice, this list
#       of conditions and the following disclaimer in the documentation and/or other materials
#       provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY MARSHMALLOW ENGINE ``AS IS'' AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
# FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL MARSHMALLOW ENGINE OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# The views and conclusions contained in the software and documentation are those of the
# authors and should not be interpreted as representing official policies, either expressed
# or implied, of Marshmallow Engine.
#

set(MARSHMALLOW_VIEWPORT_TITLE     "Marshmallow HARMATTAN_ARMEL" CACHE STRING "")
set(MARSHMALLOW_VIEWPORT_WIDTH     "854" CACHE STRING "")
set(MARSHMALLOW_VIEWPORT_HEIGHT    "480" CACHE STRING "")
set(MARSHMALLOW_VIEWPORT_DEPTH     "16"  CACHE STRING "")
set(MARSHMALLOW_VIEWPORT_VSYNC     "1"   CACHE STRING "")
set(MARSHMALLOW_VIEWPORT_FULLSCREEN ON   CACHE BOOL   "")

set(MARSHMALLOW_VIEWPORT_DUMMY      OFF  CACHE BOOL   "")
set(MARSHMALLOW_VIEWPORT_OPENGL     ON   CACHE BOOL   "")

set(MARSHMALLOW_OPENGL_EGL          ON   CACHE BOOL   "")
set(MARSHMALLOW_OPENGL_ES2          ON   CACHE BOOL   "")
set(MARSHMALLOW_OPENGL_X11          ON   CACHE BOOL   "")

set(MARSHMALLOW_X11_GRAB_POINTER      OFF CACHE BOOL  "")
set(MARSHMALLOW_X11_OVERRIDE_REDIRECT OFF CACHE BOOL  "")
set(MARSHMALLOW_INPUT_UNIX_X11        OFF CACHE BOOL  "")
set(MARSHMALLOW_INPUT_LINUX_EVDEV      ON CACHE BOOL  "")
set(MARSHMALLOW_EVDEV_KEYBOARD         ON CACHE BOOL  "")
set(MARSHMALLOW_EVDEV_JOYSTICK         ON CACHE BOOL  "")
