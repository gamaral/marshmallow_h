/*
 * Copyright 2012 Marshmallow Engine. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are
 * permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this list of
 *      conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright notice, this list
 *      of conditions and the following disclaimer in the documentation and/or other materials
 *      provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY MARSHMALLOW ENGINE ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL MARSHMALLOW ENGINE OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are those of the
 * authors and should not be interpreted as representing official policies, either expressed
 * or implied, of Marshmallow Engine.
 */

#include "evdev_eventdevice.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"

#include <linux/input.h>

#include <fcntl.h>

#include <cstdio>
#include <cstring>

#ifdef MARSHMALLOW_EVDEV_JOYSTICK
#  include "evdev_joystickdevice.h"
#endif

#ifdef MARSHMALLOW_EVDEV_KEYBOARD
#  include "evdev_keyboarddevice.h"
#endif

/*
 * EVDEV EventDevice Notes
 */

MARSHMALLOW_NAMESPACE_BEGIN
namespace Input { /****************************************** Input Namespace */
namespace Linux { /*********************************** Input::Linux Namespace */
namespace EVDEV { /**************************** Input::Linux::EVDEV Namespace */
namespace { /********************* Input::Linux::EVDEV::<anonymous> Namespace */

/*
 * Input port source ids
 *
 * I used a single 32 bit integer to identify available/used game ports, the
 * layout is the following:
 *
 * BIT  00000000 00000000 00000000 00000000
 * SLOT 12345678 12345678 12345678 12345678
 *      KEYBOARD JOYSTICK   MOUSE  RESERVED
 */
#define EVDEV_ID_MAX  8u /* 8 bits per device */
#define EVDEV_ID_KB   EVDEV_ID_MAX * 0 /* first segment */
#define EVDEV_ID_JS   EVDEV_ID_MAX * 1 /* second segment */
#define EVDEV_ID_MS   EVDEV_ID_MAX * 2 /* third segment */
#define EVDEV_ID_MISC EVDEV_ID_MAX * 3 /* forth segment */

const char s_evdev_template[] = "/dev/input/event%u";

int32_t s_evdev_ports(0); /* input ports */

} /******************************* Input::Linux::EVDEV::<anonymous> Namespace */

EventDevice::EventDevice(int fd_, Type type_)
    : m_id(0)
    , m_fd(fd_)
    , m_type(type_)
{
	/* get device name */
	ioctl(m_fd, EVIOCGNAME(EVDEV_NAME_MAX), m_name);

	/* port bit offset */
	uint8_t l_id_offset;

	switch (m_type) {
	case GamepadType:
	case JoystickType: l_id_offset = EVDEV_ID_JS; break;
	case KeyboardType: l_id_offset = EVDEV_ID_KB; break;
	case MouseType:    l_id_offset = EVDEV_ID_MS; break;
	default: return;
	}

	/* search for available port */
	const int16_t l_id = static_cast<int16_t>(s_evdev_ports >> l_id_offset);
	for (size_t i = 0; i < EVDEV_ID_MAX; ++i)
		if (0 == (l_id & (1 << i))) {
			m_id = i;
			break;
		}

	/* clear port bit */
	s_evdev_ports |= (1 << (l_id_offset + m_id));
}

EventDevice::~EventDevice(void)
{
	close();

	/* port bit offset */
	uint8_t l_id_offset;

	switch (m_type) {
	case GamepadType:
	case JoystickType: l_id_offset = EVDEV_ID_JS; break;
	case KeyboardType: l_id_offset = EVDEV_ID_KB; break;
	case MouseType:    l_id_offset = EVDEV_ID_MS; break;
	default: return;
	}

	/* clear port bit */
	s_evdev_ports ^= (1 << (l_id_offset + m_id));
}

EventDevice *
EventDevice::Open(unsigned int evdev_id, int mask)
{
	static const unsigned char KBITS_SIZE((KEY_MAX/LONG_BIT) + 1);
	static const size_t s_kbindex = KEY_ENTER / LONG_BIT;
	static const size_t s_kbbit   = KEY_ENTER % LONG_BIT;
	static const size_t s_msindex = BTN_MOUSE / LONG_BIT;
	static const size_t s_msbit   = BTN_MOUSE % LONG_BIT;
	static const size_t s_gpindex = BTN_GAMEPAD / LONG_BIT;
	static const size_t s_gpbit   = BTN_GAMEPAD % LONG_BIT;
	static const size_t s_jsindex = BTN_JOYSTICK / LONG_BIT;
	static const size_t s_jsbit   = BTN_JOYSTICK % LONG_BIT;

	char l_evdev_path[sizeof(s_evdev_template) + 4];
	snprintf(l_evdev_path, sizeof(l_evdev_path), s_evdev_template, evdev_id);

	EventDevice *l_evdev = 0;

	struct input_id l_id;
	unsigned long l_type_bits = 0;
	unsigned long l_key_bits[KBITS_SIZE];

	int  l_fd = -1;
	Type l_type = UnknownType;

	if (-1 == (l_fd = ::open(l_evdev_path, O_RDONLY|O_NONBLOCK)))
		return(0);

	/* check EV_KEY support (required) */
	ioctl(l_fd, EVIOCGBIT(0, EV_MAX), &l_type_bits);
	if (0 == (l_type_bits & EV_KEY)) {
		MMWARNING(l_evdev_path << " doesn't support EV_KEY. IGNORING.");
		return(0);
	}

	/* get key bits */
	memset(&l_key_bits, 0, sizeof(l_key_bits));
	ioctl(l_fd, EVIOCGBIT(EV_KEY, KEY_MAX), &l_key_bits);

	/*
	 * Device type detection
	 */
	if (0 != (l_key_bits[s_kbindex] & (1ul << s_kbbit))
	    && KeyboardType == (mask & KeyboardType))
		l_type = KeyboardType;
	else if (0 != (l_key_bits[s_msindex] & (1ul << s_msbit))
	    && MouseType == (mask & MouseType))
		l_type = MouseType;
	else if (0 != (l_key_bits[s_gpindex] & (1ul << s_gpbit))
	    && GamepadType == (mask & GamepadType))
		l_type = GamepadType;
	else if (0 != (l_key_bits[s_jsindex] & (1ul << s_jsbit))
	    && JoystickType == (mask & JoystickType))
		l_type = JoystickType;
	else {
		MMWARNING("Unknown event device type. IGNORING.");
		return(0);
	}

	ioctl(l_fd, EVIOCGID, &l_id);

	/*
	 * Create device handlers
	 */
	switch (l_type) {
#ifdef MARSHMALLOW_EVDEV_KEYBOARD
	case KeyboardType:
		l_evdev =
		    new KeyboardDevice(l_fd, l_type);
		break;
#endif

#ifdef MARSHMALLOW_EVDEV_JOYSTICK
	case GamepadType:
	case JoystickType:
		l_evdev =
		    new JoystickDevice(l_fd, l_type, l_id.vendor, l_id.product);
		break;
#endif

	/*
	 * TODO(gamaral) Implement
	 */
	case MouseType:
#ifndef MARSHMALLOW_EVDEV_KEYBOARD
	case KeyboardType:
#endif
#ifndef MARSHMALLOW_EVDEV_JOYSTICK
	case GamepadType:
	case JoystickType:
#endif
	default:
		MMWARNING("Event device ignored: "
		          "VENDOR["  << l_id.vendor << "] "
		          "PRODUCT[" << l_id.product << "] "
		          "VERSION[" << l_id.version << "] "
		          "TYPE[ " << l_type << "]");
		return(0);
	}

	/*
	 * SUCCESS!
	 */

	MMDEBUG("Event device opened:"
		"\""    << l_evdev->name() << "\" "
		"VENDOR["  << l_id.vendor << "] "
		"PRODUCT[" << l_id.product << "] "
		"VERSION[" << l_id.version << "] "
		"TYPE[ " << l_type << "]");

	return(l_evdev);
}

void
EventDevice::close(void)
{
	if (-1 != m_fd)
		::close(m_fd), m_fd = -1;

	MMDEBUG("Event device closed:"
		"\""    << m_name << "\" "
		"ID[ " << m_id << "]"
		"TYPE[ " << m_type << "]");
	m_name[0] = '\0';
	m_id = 0;
	m_type = UnknownType;
}

bool
EventDevice::processEvents(void)
{
	struct input_event l_event;
	static const ssize_t l_event_size(sizeof(l_event));
	bool l_handled = false;

	while (read(m_fd, &l_event, l_event_size) == l_event_size) {
		switch (m_type) {
		case KeyboardType:
			if (l_event.type != EV_KEY)
				continue;
			else break;

		case MouseType:
			if (l_event.type != EV_KEY && l_event.type != EV_REL)
				continue;
			else break;

		case GamepadType:
		case JoystickType:
			if (l_event.type != EV_KEY && l_event.type != EV_ABS)
				continue;
			else break;

		default: continue;
		}

		l_handled |= handleEvent(l_event);
	}

	return(l_handled);
}

bool
EventDevice::isValid(void) const
{
	int l_v;
	return(-1 != ioctl(m_fd, EVIOCGVERSION, &l_v));
}

} /******************************************** Input::Linux::EVDEV Namespace */
} /*************************************************** Input::Linux Namespace */
} /********************************************************** Input Namespace */
MARSHMALLOW_NAMESPACE_END

