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

#include "evdev.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"

#include "event/eventmanager.h"
#include "event/joystickaxisevent.h"
#include "event/joystickbuttonevent.h"
#include "event/keyboardevent.h"

#include "input/keyboard_p.h"
#include "input/joystick_p.h"

#include <linux/input.h>
#include <sys/inotify.h>

#include <vector>
#include <limits>

#include <fcntl.h>
#include <stdint.h>

#include <cstdio>
#include <cstring>

#include "evdev_map.h"

/*
 * EVDEV Notes
 */

MARSHMALLOW_NAMESPACE_BEGIN
namespace Input { /****************************************** Input Namespace */
namespace Linux { /*********************************** Input::Linux Namespace */
namespace { /**************************** Input::Linux::<anonymous> Namespace */

namespace ED = EventDevice;

#define EVDEV_NAME_MAX   256
#define EVDEV_NUMBER_MAX 20u
#define EVDEV_ID_MAX     32u

class LinuxEventDevice;
typedef std::map<unsigned int, LinuxEventDevice *> LinuxEventDeviceMap;

static const char s_evdev_template[] = "/dev/input/event%u";
int s_initialized =  0;
int s_inotify_fd  = -1;
int s_watch_fd    = -1;
LinuxEventDeviceMap s_ev_devices;

class LinuxEventDevice
{
	char m_name[EVDEV_NAME_MAX];
	size_t m_id;
	int m_fd;
	int m_type;

	NO_ASSIGN_COPY(LinuxEventDevice);
public:
	LinuxEventDevice(int fd, ED::Type type, char *name);

	virtual ~LinuxEventDevice(void)
	    { close(); };

	static LinuxEventDevice * Open(unsigned int evdev_id, int mask);
	void close(void);

	inline void processEvents(void);

	inline const char * name(void) const
	    { return(m_name); }

	inline size_t id(void) const
	    { return(m_id); }
	void setId(size_t id_)
	    { m_id = id_; }

	inline int fd(void) const
	    { return(m_fd); }

	inline ED::Type type(void) const
	    { return(static_cast<ED::Type>(m_type)); }

protected: /* virtual */

	virtual bool handleEvent(struct input_event &event) = 0;
};

class LinuxKeyboard : public LinuxEventDevice
{
	static int32_t s_ids;

	NO_ASSIGN_COPY(LinuxKeyboard);
public:

	LinuxKeyboard(int fd, ED::Type type, struct input_id &id, char *name);
	virtual ~LinuxKeyboard(void);

protected: /* virtual */

	VIRTUAL inline bool handleEvent(struct input_event &event);
};

class LinuxJoystick : public LinuxEventDevice
{
	static int32_t s_ids;

	ED::Map::EventCodes m_abs_map;
	ED::Map::EventCodes m_key_map;
	int m_btn_state;

	NO_ASSIGN_COPY(LinuxJoystick);
public:
	LinuxJoystick(int fd, ED::Type type, struct input_id &id, char *name);
	virtual ~LinuxJoystick(void);

protected: /* virtual */

	VIRTUAL inline bool handleEvent(struct input_event &event);
};

LinuxEventDevice::LinuxEventDevice(int fd_, ED::Type type_, char *name_)
    : m_fd(fd_)
    , m_type(type_)
{
	strcpy(m_name, name_);
}

LinuxEventDevice *
LinuxEventDevice::Open(unsigned int evdev_id, int mask)
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

	LinuxEventDevice *l_evdev = 0;

	struct input_id l_id;
	unsigned long l_type_bits = 0;
	unsigned long l_key_bits[KBITS_SIZE];

	char l_name[EVDEV_NAME_MAX];
	int l_fd;
	ED::Type l_type;

	l_fd = -1;
	l_name[0] = '\0';
	l_type = ED::UnknownType;

	if (-1 == (l_fd = ::open(l_evdev_path, O_RDONLY|O_NONBLOCK)))
		return(0);

	/* check EV_KEY support (required) */
	ioctl(l_fd, EVIOCGBIT(0, EV_MAX), &l_type_bits);
	if (0 == (l_type_bits & EV_KEY)) {
		MMWARNING(l_evdev_path << " doesn't support EV_KEY. IGNORING.");
		return(0);
	}

	/* get device name */
	ioctl(l_fd, EVIOCGNAME(sizeof(l_name)), l_name);

	/* get key bits */
	memset(&l_key_bits, 0, sizeof(l_key_bits));
	ioctl(l_fd, EVIOCGBIT(EV_KEY, KEY_MAX), &l_key_bits);

	/*
	 * Device type detection
	 */
	if (0 != (l_key_bits[s_kbindex] & (1ul << s_kbbit))
	    && ED::KeyboardType == (mask & ED::KeyboardType))
		l_type = ED::KeyboardType;
	else if (0 != (l_key_bits[s_msindex] & (1ul << s_msbit))
	    && ED::MouseType == (mask & ED::MouseType))
		l_type = ED::MouseType;
	else if (0 != (l_key_bits[s_gpindex] & (1ul << s_gpbit))
	    && ED::GamepadType == (mask & ED::GamepadType))
		l_type = ED::GamepadType;
	else if (0 != (l_key_bits[s_jsindex] & (1ul << s_jsbit))
	    && ED::JoystickType == (mask & ED::JoystickType))
		l_type = ED::JoystickType;
	else {
		MMWARNING("Unknown device type. IGNORING.");
		return(0);
	}

	ioctl(l_fd, EVIOCGID, &l_id);
	MMDEBUG("Successfully opened event device "
		"\""    << l_name << "\" "
		"VENDOR["  << l_id.vendor << "] "
		"PRODUCT[" << l_id.product << "] "
		"VERSION[" << l_id.version << "] "
		"TYPE[ " << l_type << "]");

	/*
	 * Create device handlers
	 */
	switch (l_type) {
	case ED::KeyboardType:
		l_evdev = new LinuxKeyboard(l_fd, l_type, l_id, l_name);
		break;

	case ED::GamepadType:
	case ED::JoystickType:
		l_evdev = new LinuxJoystick(l_fd, l_type, l_id, l_name);
		break;

	/*
	 * TODO(gamaral) Implement
	 */
	case ED::MouseType:
	default:
		MMINFO("Event device " << evdev_id << " identified as \""
		    << l_name << "\" was ignored.");
		return(0);
	}

	s_ev_devices[evdev_id] = l_evdev;

	return(l_evdev);
}

void
LinuxEventDevice::close(void)
{
	if (-1 != m_fd)
		::close(m_fd), m_fd = -1;

	m_name[0] = '\0';
	m_id = 0;
	m_type = ED::UnknownType;
}

void
LinuxEventDevice::processEvents(void)
{
	struct input_event l_event;
	static const size_t l_event_size(sizeof(l_event));

	while (read(m_fd, &l_event, l_event_size) == l_event_size) {
		switch (m_type) {
		case ED::KeyboardType:
			if (l_event.type != EV_KEY)
				continue;
			else break;

		case ED::MouseType:
			if (l_event.type != EV_KEY && l_event.type != EV_REL)
				continue;
			else break;

		case ED::GamepadType:
		case ED::JoystickType:
			if (l_event.type != EV_KEY && l_event.type != EV_ABS)
				continue;
			else break;

		default: continue;
		}

		handleEvent(l_event);
	}
}

int32_t LinuxKeyboard::s_ids(0);

LinuxKeyboard::LinuxKeyboard(int fd_, ED::Type type_,
                             struct input_id &, char *name_)
    : LinuxEventDevice(fd_, type_, name_)
{
	/* TODO(gamaral) handle override map */

	/* look for available id */
	for (size_t i = 0; i < EVDEV_ID_MAX && id() == 0; ++i)
		if (0 == (s_ids & (1 << i)))
			setId(i);
	s_ids |= (1 << id());

	MMDEBUG("Keyboard slot id " << id() << " assigned.");
}

LinuxKeyboard::~LinuxKeyboard(void)
{
	s_ids ^= (1 << id());
}

bool
LinuxKeyboard::handleEvent(struct input_event &event)
{
	using namespace Event;

	Keyboard::Key l_key = Keyboard::KBK_NONE;
	Keyboard::Action l_action;

	switch (event.code) {
	case KEY_BACKSPACE:   l_key = Keyboard::KBK_BACKSPACE; break;
	case KEY_TAB:         l_key = Keyboard::KBK_TAB; break;
	case KEY_CLEAR:       l_key = Keyboard::KBK_CLEAR; break;
	case KEY_ENTER:       l_key = Keyboard::KBK_ENTER; break;
	case KEY_PAUSE:       l_key = Keyboard::KBK_PAUSE; break;
	case KEY_ESC:         l_key = Keyboard::KBK_ESCAPE; break;

	case KEY_SPACE:       l_key = Keyboard::KBK_SPACE; break;
	case KEY_COMMA:       l_key = Keyboard::KBK_COMMA; break;
	case KEY_MINUS:       l_key = Keyboard::KBK_MINUS; break;
	case KEY_DOT:         l_key = Keyboard::KBK_DOT; break;
	case KEY_SLASH:       l_key = Keyboard::KBK_SLASH; break;

	case KEY_0:           l_key = Keyboard::KBK_0; break;
	case KEY_1:           l_key = Keyboard::KBK_1; break;
	case KEY_2:           l_key = Keyboard::KBK_2; break;
	case KEY_3:           l_key = Keyboard::KBK_3; break;
	case KEY_4:           l_key = Keyboard::KBK_4; break;
	case KEY_5:           l_key = Keyboard::KBK_5; break;
	case KEY_6:           l_key = Keyboard::KBK_6; break;
	case KEY_7:           l_key = Keyboard::KBK_7; break;
	case KEY_8:           l_key = Keyboard::KBK_8; break;
	case KEY_9:           l_key = Keyboard::KBK_9; break;

	case KEY_SEMICOLON:   l_key = Keyboard::KBK_SEMICOLON; break;
	case KEY_EQUAL:       l_key = Keyboard::KBK_EQUAL; break;

	case KEY_A:           l_key = Keyboard::KBK_A; break;
	case KEY_B:           l_key = Keyboard::KBK_B; break;
	case KEY_C:           l_key = Keyboard::KBK_C; break;
	case KEY_D:           l_key = Keyboard::KBK_D; break;
	case KEY_E:           l_key = Keyboard::KBK_E; break;
	case KEY_F:           l_key = Keyboard::KBK_F; break;
	case KEY_G:           l_key = Keyboard::KBK_G; break;
	case KEY_H:           l_key = Keyboard::KBK_H; break;
	case KEY_I:           l_key = Keyboard::KBK_I; break;
	case KEY_J:           l_key = Keyboard::KBK_J; break;
	case KEY_K:           l_key = Keyboard::KBK_K; break;
	case KEY_L:           l_key = Keyboard::KBK_L; break;
	case KEY_M:           l_key = Keyboard::KBK_M; break;
	case KEY_N:           l_key = Keyboard::KBK_N; break;
	case KEY_O:           l_key = Keyboard::KBK_O; break;
	case KEY_P:           l_key = Keyboard::KBK_P; break;
	case KEY_Q:           l_key = Keyboard::KBK_Q; break;
	case KEY_R:           l_key = Keyboard::KBK_R; break;
	case KEY_S:           l_key = Keyboard::KBK_S; break;
	case KEY_T:           l_key = Keyboard::KBK_T; break;
	case KEY_U:           l_key = Keyboard::KBK_U; break;
	case KEY_V:           l_key = Keyboard::KBK_V; break;
	case KEY_W:           l_key = Keyboard::KBK_W; break;
	case KEY_X:           l_key = Keyboard::KBK_X; break;
	case KEY_Y:           l_key = Keyboard::KBK_Y; break;
	case KEY_Z:           l_key = Keyboard::KBK_Z; break;

	case KEY_LEFTBRACE:   l_key = Keyboard::KBK_BRACKET_L; break;
	case KEY_BACKSLASH:   l_key = Keyboard::KBK_BACKSLASH; break;
	case KEY_RIGHTBRACE:  l_key = Keyboard::KBK_BRACKET_R; break;
	case KEY_GRAVE:       l_key = Keyboard::KBK_GRAVE; break;

	case KEY_LEFTSHIFT:   l_key = Keyboard::KBK_SHIFT_L; break;
	case KEY_RIGHTSHIFT:  l_key = Keyboard::KBK_SHIFT_R; break;
	case KEY_CAPSLOCK:    l_key = Keyboard::KBK_CAPS_LOCK; break;

	case KEY_LEFTALT:     l_key = Keyboard::KBK_ALT_L; break;
	case KEY_RIGHTALT:    l_key = Keyboard::KBK_ALT_R; break;

	case KEY_LEFTCTRL:    l_key = Keyboard::KBK_CONTROL_L; break;
	case KEY_RIGHTCTRL:   l_key = Keyboard::KBK_CONTROL_R; break;

	case KEY_LEFTMETA:    l_key = Keyboard::KBK_META_L; break;
	case KEY_RIGHTMETA:   l_key = Keyboard::KBK_META_R; break;

	case KEY_F1:          l_key = Keyboard::KBK_F1; break;
	case KEY_F2:          l_key = Keyboard::KBK_F2; break;
	case KEY_F3:          l_key = Keyboard::KBK_F3; break;
	case KEY_F4:          l_key = Keyboard::KBK_F4; break;
	case KEY_F5:          l_key = Keyboard::KBK_F5; break;
	case KEY_F6:          l_key = Keyboard::KBK_F6; break;
	case KEY_F7:          l_key = Keyboard::KBK_F7; break;
	case KEY_F8:          l_key = Keyboard::KBK_F8; break;
	case KEY_F9:          l_key = Keyboard::KBK_F9; break;
	case KEY_F10:         l_key = Keyboard::KBK_F10; break;
	case KEY_F11:         l_key = Keyboard::KBK_F11; break;
	case KEY_F12:         l_key = Keyboard::KBK_F12; break;
	case KEY_F13:         l_key = Keyboard::KBK_F13; break;
	case KEY_F14:         l_key = Keyboard::KBK_F14; break;
	case KEY_F15:         l_key = Keyboard::KBK_F15; break;

	case KEY_PRINT:       l_key = Keyboard::KBK_PRINT; break;
	case KEY_SCROLLLOCK:  l_key = Keyboard::KBK_SCROLL_LOCK; break;
	case KEY_BREAK:       l_key = Keyboard::KBK_BREAK; break;

	case KEY_INSERT:      l_key = Keyboard::KBK_INSERT; break;
	case KEY_DELETE:      l_key = Keyboard::KBK_DELETE; break;
	case KEY_HOME:        l_key = Keyboard::KBK_HOME; break;
	case KEY_END:         l_key = Keyboard::KBK_END; break;
	case KEY_PAGEUP:      l_key = Keyboard::KBK_PAGE_UP; break;
	case KEY_PAGEDOWN:    l_key = Keyboard::KBK_PAGE_DOWN; break;

	case KEY_UP:          l_key = Keyboard::KBK_UP; break;
	case KEY_DOWN:        l_key = Keyboard::KBK_DOWN; break;
	case KEY_LEFT:        l_key = Keyboard::KBK_LEFT; break;
	case KEY_RIGHT:       l_key = Keyboard::KBK_RIGHT; break;

	case KEY_NUMLOCK:     l_key = Keyboard::KBK_NUM_LOCK; break;
	case KEY_KPSLASH:     l_key = Keyboard::KBK_KSLASH; break;
	case KEY_KPASTERISK:  l_key = Keyboard::KBK_KASTERISK; break;
	case KEY_KPMINUS:     l_key = Keyboard::KBK_KMINUS; break;
	case KEY_KP7:         l_key = Keyboard::KBK_K7; break;
	case KEY_KP8:         l_key = Keyboard::KBK_K8; break;
	case KEY_KP9:         l_key = Keyboard::KBK_K9; break;
	case KEY_KP4:         l_key = Keyboard::KBK_K4; break;
	case KEY_KP5:         l_key = Keyboard::KBK_K5; break;
	case KEY_KP6:         l_key = Keyboard::KBK_K6; break;
	case KEY_KPPLUS:      l_key = Keyboard::KBK_KPLUS; break;
	case KEY_KP1:         l_key = Keyboard::KBK_K1; break;
	case KEY_KP2:         l_key = Keyboard::KBK_K2; break;
	case KEY_KP3:         l_key = Keyboard::KBK_K3; break;
	case KEY_KP0:         l_key = Keyboard::KBK_K0; break;
	case KEY_KPDOT:       l_key = Keyboard::KBK_KDOT; break;
	case KEY_KPENTER:     l_key = Keyboard::KBK_KENTER; break;

	case KEY_CLOSE:       l_key = Keyboard::KBK_CLOSE; break;
	case KEY_HELP:        l_key = Keyboard::KBK_HELP; break;
	case KEY_MENU:        l_key = Keyboard::KBK_MENU; break;

	default:
		MMDEBUG("Unknown key code: " << event.code
		    << " name[" << name() << "]"
		    << " id["   << id()   << "]");
		return(false);
	}

	/* translate action */
	switch(event.value) {
	case 0: l_action = Keyboard::KeyReleased; break;
	case 1: l_action = Keyboard::KeyPressed; break;
	default: return(false);
	}

	const Keyboard::Action l_prev_action = Keyboard::KeyState(l_key);
	if (l_prev_action != l_action) {
		Keyboard::SetKeyState(l_key, l_action);

		SharedEvent l_event(new KeyboardEvent(l_key, l_action, id()));
		EventManager::Instance()->queue(l_event);
	}

	return(true);
}

int32_t LinuxJoystick::s_ids(0);

LinuxJoystick::LinuxJoystick(int fd_, ED::Type type_,
                             struct input_id &id_, char *name_)
    : LinuxEventDevice(fd_, type_, name_)
    , m_btn_state(0)
{
	/* look for available id */
	for (size_t i = 0; i < EVDEV_ID_MAX && id() == 0; ++i)
		if (0 == (s_ids & (1 << i)))
			setId(i);
	s_ids |= (1 << id());

	MMDEBUG("Joystick slot id " << id() << " assigned.");

	ED::Map::PopulateEventCodes(id_.vendor, id_.product, name(),
	    type(), EV_ABS, m_abs_map);
	ED::Map::PopulateEventCodes(id_.vendor, id_.product, name(),
	    type(), EV_KEY, m_key_map);
}

LinuxJoystick::~LinuxJoystick(void)
{
	s_ids ^= (1 << id());
}

bool
LinuxJoystick::handleEvent(struct input_event &event)
{
	using namespace Event;

	if (event.type == EV_KEY) {
		ED::Map::EventCodes::const_iterator l_entry =
		    m_key_map.find(event.code);
		if (l_entry == m_key_map.end())
			return(false);

		Joystick::Action l_action;
		
		if (0 == event.value) {
			m_btn_state &= ~(l_entry->second);
			l_action = Joystick::ButtonReleased;
		}
		else {
			m_btn_state |= l_entry->second;
			l_action = Joystick::ButtonPressed;
		}

		Joystick::Button l_btn = static_cast<Joystick::Button>(l_entry->second);

		/*
		 *  HAT ABS Emulation
		 *
		 */
		if (Joystick::JSB_LEFT == l_btn || Joystick::JSB_RIGHT == l_btn
		    || Joystick::JSB_UP == l_btn || Joystick::JSB_DOWN == l_btn) {

			Joystick::Axis l_axis = Joystick::JSA_NONE;
			int l_value = event.value;

			switch(l_btn) {
			case Joystick::JSB_LEFT:
				l_value = -l_value;
				/* fallthrough */
			case Joystick::JSB_RIGHT:
				l_axis = Joystick::JSA_HX;
				break;
			case Joystick::JSB_UP:
				l_value = -l_value;
				/* fallthrough */
			case Joystick::JSB_DOWN:
				l_axis = Joystick::JSA_HY;
				break;
			default: break;
			}

			SharedEvent l_event(new JoystickAxisEvent(
			    l_axis,
			    l_value, -1, 1, 0, 0,
			    id()));
			EventManager::Instance()->queue(l_event);
		}

		SharedEvent l_event(new JoystickButtonEvent(
		    l_btn,
		    l_action,
		    m_btn_state,
		    id()));
		EventManager::Instance()->queue(l_event);
	}
	else if (event.type == EV_ABS) {
		ED::Map::EventCodes::const_iterator l_entry =
		    m_abs_map.find(event.code);
		if (l_entry == m_abs_map.end())
			return(false);

		struct input_absinfo l_absinfo;
		memset(&l_absinfo, 0, sizeof(l_absinfo));
		ioctl(fd(), EVIOCGABS(event.code), &l_absinfo);

		SharedEvent l_event(new JoystickAxisEvent(
		    static_cast<Joystick::Axis>(l_entry->second),
		    l_absinfo.value,
		    l_absinfo.minimum,
		    l_absinfo.maximum,
		    l_absinfo.fuzz,
		    l_absinfo.flat,
		    id()));
		EventManager::Instance()->queue(l_event);
	}
	else return(false);
	
	return(true);
}

void
ProcessNotificationEvents(void)
{
	static const ssize_t l_event_size(sizeof(struct inotify_event));
	char l_raw[l_event_size + NAME_MAX + 1];
	struct inotify_event *l_event;
	ssize_t l_c, l_i = 0;
	unsigned int l_evdev_id;

	/*
	 * Fetch notifications
	 */
	while ((l_c = read(s_inotify_fd, &l_raw, sizeof(l_raw))) >= l_event_size) {
		/*
		 * Process all notifications in buffer
		 */
		do {
			l_event =
			    reinterpret_cast<struct inotify_event *>(&l_raw[l_i]);

			/*
			 * Get event device id, everything else is ignored.
			 * (usually js* change notifications)
			 */
			if (1 != sscanf(l_event->name, "event%ud", &l_evdev_id))
				continue;

			if (IN_CREATE == (l_event->mask & IN_CREATE))
				LinuxEventDevice::Open(l_evdev_id, s_initialized);
			else if (IN_DELETE == (l_event->mask & IN_DELETE)) {
				LinuxEventDeviceMap::iterator l_ei;
				l_ei = s_ev_devices.find(l_evdev_id);
				if (s_ev_devices.end() == l_ei)
					continue;
				delete l_ei->second;
				s_ev_devices.erase(l_ei);
			}
		}
		while ((l_i += l_event_size + l_event->len) < l_c);
	}
}

bool
Initialize(int mask)
{
	/*
	 *  Make sure we actually initialize once, and only once.
	 */
	if (0 == s_initialized) {
		s_inotify_fd = inotify_init1(O_NONBLOCK);
		s_watch_fd = inotify_add_watch(s_inotify_fd, "/dev/input",
		                               IN_CREATE|IN_DELETE);
		if (-1 == s_watch_fd)
			MMWARNING("INotify watch request failed.");

		ED::Map::Initialize();
	}

	/* TODO(gamaral) process /proc/bus/input/devices */
	for (unsigned int i = 0; i < EVDEV_NUMBER_MAX; ++i)
		LinuxEventDevice::Open(i, mask);

	s_initialized |= mask;

	return(s_ev_devices.size() > 0);
}

void
Finalize(int mask)
{
	LinuxEventDeviceMap::reverse_iterator i;
	LinuxEventDeviceMap::const_reverse_iterator c = s_ev_devices.rend();
	for (i = s_ev_devices.rbegin(); i != c;) {
		if (0 != (mask & i->second->type())) {
			delete i->second;
			s_ev_devices.erase((++i).base());
		}
		else ++i;
	}

	s_initialized &= ~(mask);

	/* truly finalize if we closed all devices */
	if (0 != s_initialized)
		return;
	assert(0 == s_ev_devices.size() && "We still have open devices!");

	if (-1 == s_watch_fd)
		inotify_rm_watch(s_inotify_fd, s_watch_fd);
	close(s_inotify_fd);

	ED::Map::Finalize();
}

void
Tick(int mask)
{
	ProcessNotificationEvents();

	LinuxEventDeviceMap::iterator i;
	LinuxEventDeviceMap::const_iterator c = s_ev_devices.end();
	for (i = s_ev_devices.begin(); i != c; ++i)
		if (0 != (mask & i->second->type()))
			i->second->processEvents();
}

} /************************************** Input::Linux::<anonymous> Namespace */

bool
EventDevice::InitializeKeyboard(void)
{
	return(Initialize(KeyboardType));
}

void
EventDevice::FinalizeKeyboard(void)
{
	Finalize(KeyboardType);
}

void
EventDevice::TickKeyboard(void)
{
	Tick(KeyboardType);
}

bool
EventDevice::InitializeJoystick(void)
{
	return(Initialize(JoystickType|GamepadType));
}

void
EventDevice::FinalizeJoystick(void)
{
	Finalize(JoystickType|GamepadType);
}

void
EventDevice::TickJoystick(void)
{
	Tick(JoystickType|GamepadType);
}

} /*************************************************** Input::Linux Namespace */
} /********************************************************** Input Namespace */
MARSHMALLOW_NAMESPACE_END

