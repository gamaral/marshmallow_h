###################################################################### OPTIONS #

if(UNIX AND ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	option(MARSHMALLOW_INPUT_LINUX_EVDEV "Linux event device support" OFF)
endif()

# graphics backend dependent
if(MARSHMALLOW_OPENGL_BB)
	option(MARSHMALLOW_INPUT_BB_SCREEN "BlackBerry screen input event support" ON)
	option(MARSHMALLOW_INPUT_BB_SENSOR "BlackBerry sensor input event support" ON)

	if(MARSHMALLOW_INPUT_BB_SENSOR)
		option(MARSHMALLOW_INPUT_BB_ACCELEROMETER "BlackBerry accelerometer support"        OFF)
		option(MARSHMALLOW_INPUT_BB_GRAVITY       "BlackBerry gravity sensor support"       OFF)
		option(MARSHMALLOW_INPUT_BB_GYROSCOPE     "BlackBerry gyroscope support"            OFF)
		option(MARSHMALLOW_INPUT_BB_LINEAR_ACCEL  "BlackBerry linear accelerometer support" OFF)
		option(MARSHMALLOW_INPUT_BB_MAGNETOMETER  "BlackBerry magnetometer support"         OFF)
	endif()
elseif(MARSHMALLOW_OPENGL_QT4)
	option(MARSHMALLOW_INPUT_MISC_QT4 "Qt4 input event support" ON)
elseif(MARSHMALLOW_OPENGL_SDL)
	option(MARSHMALLOW_INPUT_MISC_SDL "SDL input event support" ON)
elseif(MARSHMALLOW_OPENGL_WGL)
	option(MARSHMALLOW_INPUT_W32_SYSTEM "Windows input event support" ON)
elseif(MARSHMALLOW_OPENGL_X11)
	option(MARSHMALLOW_INPUT_UNIX_X11 "X11 input event support" ON)
endif()

################################################################################

add_definitions(-DMARSHMALLOW_INPUT_LIBRARY)

file(GLOB MARSHMALLOW_INPUT_SRCS "*.cpp")

set(MARSHMALLOW_INPUT_LIBS "marshmallow_core"
                           "marshmallow_event"
)

if(MARSHMALLOW_INPUT_LINUX_EVDEV)
	option(MARSHMALLOW_EVDEV_KEYBOARD "Linux event device keyboard support" ON)
	option(MARSHMALLOW_EVDEV_JOYSTICK "Linux event device joystick support" ON)

	add_definitions(-DMARSHMALLOW_INPUT_LINUX_EVDEV)
	list(APPEND MARSHMALLOW_INPUT_SRCS "linux/evdev.cpp"
	                                   "linux/evdev_map.cpp"
	                                   "linux/evdev_eventdevice.cpp"
	)

	if(MARSHMALLOW_EVDEV_KEYBOARD)
		add_definitions(-DMARSHMALLOW_EVDEV_KEYBOARD)
		list(APPEND MARSHMALLOW_INPUT_SRCS "linux/evdev_keyboarddevice.cpp")
	endif()

	if(MARSHMALLOW_EVDEV_JOYSTICK)
		add_definitions(-DMARSHMALLOW_EVDEV_JOYSTICK)
		list(APPEND MARSHMALLOW_INPUT_SRCS "linux/evdev_joystickdevice.cpp")
	endif()

	if(NOT MARSHMALLOW_LIBRARY_ONLY)
		install(FILES "${PROJECT_SOURCE_DIR}/resources/linux/evdev.xml"
		        DESTINATION .
		        COMPONENT deployment)
	endif()
endif()

if(MARSHMALLOW_INPUT_UNIX_X11 AND MARSHMALLOW_OPENGL_X11)
	list(APPEND MARSHMALLOW_INPUT_SRCS "unix/x11.cpp")

	add_definitions(-DMARSHMALLOW_INPUT_UNIX_X11)
endif()

if(MARSHMALLOW_OPENGL_QT4 AND MARSHMALLOW_INPUT_MISC_QT4)
	find_package(Qt4 COMPONENTS QtCore QtGui)
	include(${QT_USE_FILE})
	include_directories(${QT_INCLUDE_DIR})

	list(APPEND MARSHMALLOW_INPUT_SRCS "misc/qt4.cpp")

	add_definitions(-DMARSHMALLOW_INPUT_MISC_QT4)
endif()

if(MARSHMALLOW_OPENGL_SDL AND MARSHMALLOW_INPUT_MISC_SDL)
	find_package(SDL)
	include_directories(${SDL_INCLUDE_DIR})

	list(APPEND MARSHMALLOW_INPUT_SRCS "misc/sdl.cpp")

	add_definitions(-DMARSHMALLOW_INPUT_MISC_SDL)
endif()

if(MARSHMALLOW_OPENGL_WGL AND MARSHMALLOW_INPUT_W32_SYSTEM)
	list(APPEND MARSHMALLOW_INPUT_SRCS "w32/system.cpp")

	add_definitions(-DMARSHMALLOW_INPUT_W32_SYSTEM)
endif()

if(MARSHMALLOW_OPENGL_BB)
	if(MARSHMALLOW_INPUT_BB_SCREEN)
		list(APPEND MARSHMALLOW_INPUT_SRCS "bb/screen.cpp")

		add_definitions(-DMARSHMALLOW_INPUT_BB_SCREEN)
	endif()

	if(MARSHMALLOW_INPUT_BB_SENSOR)
		add_definitions(-DMARSHMALLOW_INPUT_BB_SENSOR)

		if(MARSHMALLOW_INPUT_BB_ACCELEROMETER)
			add_definitions(-DMARSHMALLOW_INPUT_BB_ACCELEROMETER)
		endif()

		if(MARSHMALLOW_INPUT_BB_GRAVITY)
			add_definitions(-DMARSHMALLOW_INPUT_BB_GRAVITY)
		endif()

		if(MARSHMALLOW_INPUT_BB_GYROSCOPE)
			add_definitions(-DMARSHMALLOW_INPUT_BB_GYROSCOPE)
		endif()

		if(MARSHMALLOW_INPUT_BB_LINEAR_ACCEL)
			add_definitions(-DMARSHMALLOW_INPUT_BB_LINEAR_ACCEL)
		endif()

		if(MARSHMALLOW_INPUT_BB_MAGNETOMETER)
			add_definitions(-DMARSHMALLOW_INPUT_BB_MAGNETOMETER)
		endif()

		list(APPEND MARSHMALLOW_INPUT_SRCS "bb/sensor.cpp")
	endif()
endif()

# tinyxml
include_directories(${TINYXML2_INCLUDE_DIR})
list(APPEND MARSHMALLOW_INPUT_LIBS ${TINYXML2_LIBRARY})

add_library(marshmallow_input ${MARSHMALLOW_INPUT_SRCS})

target_link_libraries(marshmallow_input ${MARSHMALLOW_INPUT_LIBS})

install(TARGETS marshmallow_input
        LIBRARY DESTINATION lib COMPONENT runtime
        ARCHIVE DESTINATION lib COMPONENT development)

