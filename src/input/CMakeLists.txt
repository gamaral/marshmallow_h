add_definitions(-DMARSHMALLOW_INPUT_LIBRARY)

file(GLOB MARSHMALLOW_INPUT_SRCS *.cpp)
set(MARSHMALLOW_INPUT_LIBS marshmallow_core marshmallow_event)

if(MARSHMALLOW_INPUT_LINUX_EVDEV)
	add_definitions(-DMARSHMALLOW_INPUT_LINUX_EVDEV)
	set(MARSHMALLOW_INPUT_SRCS ${MARSHMALLOW_INPUT_SRCS}
	                           linux/evdev.cpp
	                           linux/evdev_map.cpp
	                           linux/evdev_eventdevice.cpp)

	if(MARSHMALLOW_EVDEV_KEYBOARD)
		add_definitions(-DMARSHMALLOW_EVDEV_KEYBOARD)
		set(MARSHMALLOW_INPUT_SRCS ${MARSHMALLOW_INPUT_SRCS}
		                           linux/evdev_keyboarddevice.cpp)
	endif()

	if(MARSHMALLOW_EVDEV_JOYSTICK)
		add_definitions(-DMARSHMALLOW_EVDEV_JOYSTICK)
		set(MARSHMALLOW_INPUT_SRCS ${MARSHMALLOW_INPUT_SRCS}
		                           linux/evdev_joystickdevice.cpp)
	endif()

	if(NOT MARSHMALLOW_LIBRARY_ONLY)
		install(FILES ${PROJECT_SOURCE_DIR}/resources/linux/evdev.xml
		        DESTINATION .
		        COMPONENT deployment)
	endif()
endif()

if(MARSHMALLOW_INPUT_UNIX_X11 AND MARSHMALLOW_OPENGL_X11)
	add_definitions(-DMARSHMALLOW_INPUT_UNIX_X11)
	set(MARSHMALLOW_INPUT_SRCS ${MARSHMALLOW_INPUT_SRCS} unix/x11.cpp)
endif()

if(MARSHMALLOW_OPENGL_QT4 AND MARSHMALLOW_INPUT_MISC_QT4)
	find_package(Qt4 COMPONENTS QtCore QtGui)
	include(${QT_USE_FILE})
	include_directories(${QT_INCLUDE_DIR})

	add_definitions(-DMARSHMALLOW_INPUT_MISC_QT4)
	set(MARSHMALLOW_INPUT_SRCS ${MARSHMALLOW_INPUT_SRCS} misc/qt4.cpp)
endif()

if(MARSHMALLOW_OPENGL_SDL AND MARSHMALLOW_INPUT_MISC_SDL)
	find_package(SDL)
	include_directories(${SDL_INCLUDE_DIR})

	add_definitions(-DMARSHMALLOW_INPUT_MISC_SDL)
	set(MARSHMALLOW_INPUT_SRCS ${MARSHMALLOW_INPUT_SRCS} misc/sdl.cpp)
endif()

if(MARSHMALLOW_OPENGL_WGL AND MARSHMALLOW_INPUT_W32_SYSTEM)
	add_definitions(-DMARSHMALLOW_INPUT_W32_SYSTEM)
	set(MARSHMALLOW_INPUT_SRCS ${MARSHMALLOW_INPUT_SRCS} w32/system.cpp)
endif()

if(MARSHMALLOW_OPENGL_BB)
	if(MARSHMALLOW_INPUT_BB_SCREEN)
		add_definitions(-DMARSHMALLOW_INPUT_BB_SCREEN)
		set(MARSHMALLOW_INPUT_SRCS ${MARSHMALLOW_INPUT_SRCS} bb/screen.cpp)
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

		set(MARSHMALLOW_INPUT_SRCS ${MARSHMALLOW_INPUT_SRCS} bb/sensor.cpp)
	endif()
endif()

add_library(marshmallow_input ${MARSHMALLOW_INPUT_SRCS})

target_link_libraries(marshmallow_input ${MARSHMALLOW_INPUT_LIBS})

install(TARGETS marshmallow_input
        LIBRARY DESTINATION lib COMPONENT runtime
        ARCHIVE DESTINATION lib COMPONENT development)

