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

#include "input/linux/evdev.h"

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

#include <vector>

#include <fcntl.h>
#include <stdint.h>

#include <cstdio>
#include <cstring>

/*
 * EVDEV Notes
 *
 */

MARSHMALLOW_NAMESPACE_BEGIN
namespace { /******************************************** Anonymous Namespace */

	class LinuxEventDevice;
	typedef std::vector<LinuxEventDevice *> LinuxEventDeviceList;
	typedef std::map<uint16_t, int> InputMap;

	LinuxEventDeviceList s_js_devices;
	LinuxEventDeviceList s_kbd_devices;

	class LinuxEventDevice
	{
		Core::Identifier m_id;
		int m_fd;
		int m_type;

		NO_ASSIGN_COPY(LinuxEventDevice);
	public:

		enum DeviceType
		{
			dtUnknown  = 0,
			dtKeyboard = (1 << 0),
			dtMouse    = (1 << 1),
			dtJoystick = (1 << 2),
			dtGamepad  = dtJoystick,
			dtEverything = ~(1)
		};

	public:
		LinuxEventDevice(void)
		    : m_fd(-1) {}

		virtual ~LinuxEventDevice(void)
		    { close(); };

		void close(void);

		inline void processEvents(void);

		inline Core::Identifier id(void) const
		    { return(m_id); }

		inline int fd(void) const
		    { return(m_fd); }

		inline DeviceType type(void) const
		    { return(static_cast<DeviceType>(m_type)); }

	protected: /* virtual */

		virtual bool handleEvent(struct input_event &event) = 0;
		virtual void prepare(void) = 0;

	protected:

		bool open(const char *evdev, int filter = dtEverything);
	};

	class LinuxKeyboard : public LinuxEventDevice
	{
		NO_ASSIGN_COPY(LinuxKeyboard);
	public:

		LinuxKeyboard(void)
		    {}

		virtual ~LinuxKeyboard(void)
		    {}

		inline bool open(const char *evdev)
		    { return(LinuxEventDevice::open(evdev, dtKeyboard)); }

	protected: /* virtual */

		VIRTUAL inline bool handleEvent(struct input_event &event);
		VIRTUAL inline void prepare(void) {};
	};

	class LinuxJoystick : public LinuxEventDevice
	{
		InputMap m_abs_map;
		InputMap m_btn_map;
		int m_btn_state;
	
		NO_ASSIGN_COPY(LinuxJoystick);
	public:
		LinuxJoystick(void)
		    {}

		virtual ~LinuxJoystick(void)
		    {}

		inline bool open(const char *evdev)
		    { return(LinuxEventDevice::open(evdev, dtJoystick)); }

	protected: /* virtual */

		VIRTUAL inline bool handleEvent(struct input_event &event);
		VIRTUAL inline void prepare(void);
	};

	bool
	LinuxEventDevice::open(const char *evdev, int filter)
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

		char l_name[256] = "Unknown";
		unsigned short l_id[4];
		unsigned long l_type_bits = 0;
		unsigned long l_key_bits[KBITS_SIZE];

		close();

		if (-1 == (m_fd = ::open(evdev, O_RDONLY|O_NONBLOCK)))
			return(false);

		/* check EV_KEY support (required) */
		ioctl(m_fd, EVIOCGBIT(0, EV_MAX), &l_type_bits);
		if (0 == (l_type_bits & EV_KEY)) {
			MMWARNING(evdev << " doesn't support EV_KEY. IGNORING.");
			return(false);
		}

		ioctl(m_fd, EVIOCGNAME(sizeof(l_name)), l_name);
		ioctl(m_fd, EVIOCGID,  l_id);

		memset(&l_key_bits, 0, sizeof(l_key_bits));
		ioctl(m_fd, EVIOCGBIT(EV_KEY, KEY_MAX), &l_key_bits);

		if (0 != (l_key_bits[s_kbindex] & (1ul << s_kbbit))
		    && dtKeyboard == (filter & dtKeyboard))
			m_type = dtKeyboard;
		else if (0 != (l_key_bits[s_msindex] & (1ul << s_msbit))
		    && dtMouse == (filter & dtMouse))
			m_type = dtMouse;
		else if (0 != (l_key_bits[s_gpindex] & (1ul << s_gpbit))
		    && dtGamepad == (filter & dtGamepad))
			m_type = dtGamepad;
		else if (0 != (l_key_bits[s_jsindex] & (1ul << s_jsbit))
		    && dtJoystick == (filter & dtJoystick))
			m_type = dtJoystick;
		else {
			MMWARNING("Unknown device type. IGNORING.");
			return(false);
		}

		MMDEBUG("Successfully opened event device "
			"\""    << l_name << "\" "
			"BUS["     << l_id[ID_BUS]     << "] "
			"VENDOR["  << l_id[ID_VENDOR]  << "] "
			"PRODUCT[" << l_id[ID_PRODUCT] << "] "
			"VERSION[" << l_id[ID_VERSION] << "] "
			"TYPE[ " << m_type << "]");

		m_id = l_name;

		prepare();

		return(true);
	}

	void
	LinuxEventDevice::close(void)
	{
		if (-1 != m_fd)
			::close(m_fd), m_fd = -1;

		m_id = Core::Identifier();
		m_type = dtUnknown;
	}

	void
	LinuxEventDevice::processEvents(void)
	{
		struct input_event l_event;
		static const size_t l_event_size(sizeof(l_event));

		while (read(m_fd, &l_event, l_event_size) == l_event_size) {
			switch (m_type) {
			case dtKeyboard:
				if (l_event.type != EV_KEY)
					continue;
				else break;

			case dtMouse:
				if (l_event.type != EV_KEY && l_event.type != EV_REL)
					continue;
				else break;

			case dtJoystick:
				if (l_event.type != EV_KEY && l_event.type != EV_ABS)
					continue;
				else break;

			default: continue;
			}

			handleEvent(l_event);
		}

	}

	bool
	LinuxKeyboard::handleEvent(struct input_event &event)
	{
		using namespace Event;
		using namespace Input;

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

		case KEY_LEFTBRACE:   l_key = Keyboard::KBK_BRACKET_LEFT; break;
		case KEY_BACKSLASH:   l_key = Keyboard::KBK_BACKSLASH; break;
		case KEY_RIGHTBRACE:  l_key = Keyboard::KBK_BRACKET_RIGHT; break;
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
			MMDEBUG("Unknown key code: " << event.code << " id[" << id().str() << "]");
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

	void
	LinuxJoystick::prepare(void)
	{
		using namespace Input;

		m_btn_state = 0;

		/*
		 * TEMPORARY: 046d:c21a Logitech, Inc. Precision Gamepad
		 */

#ifdef LOGITECH
		m_abs_map[0] = Joystick::JSA_HX;
		m_abs_map[1] = Joystick::JSA_HY;

		m_btn_map[289] = Joystick::JSB_B;
		m_btn_map[290] = Joystick::JSB_A;
		m_btn_map[288] = Joystick::JSB_Y;
		m_btn_map[291] = Joystick::JSB_X;
		m_btn_map[292] = Joystick::JSB_L;
		m_btn_map[293] = Joystick::JSB_R;
		m_btn_map[294] = Joystick::JSB_THUMB_L;
		m_btn_map[295] = Joystick::JSB_THUMB_R;
		m_btn_map[296] = Joystick::JSB_SELECT;
		m_btn_map[297] = Joystick::JSB_START;
#else
		/*
		 * TEMPORARY: 1bad:f901 Xbox 360 Controller
		 */

		m_abs_map[16] = Joystick::JSA_HX;
		m_abs_map[17] = Joystick::JSA_HY;
		m_abs_map[0]  = Joystick::JSA_X;
		m_abs_map[1]  = Joystick::JSA_Y;
		m_abs_map[2]  = Joystick::JSA_Z;
		m_abs_map[3]  = Joystick::JSA_RX;
		m_abs_map[4]  = Joystick::JSA_RY;
		m_abs_map[5]  = Joystick::JSA_RZ;

		m_btn_map[304] = Joystick::JSB_B;
		m_btn_map[305] = Joystick::JSB_A;
		m_btn_map[307] = Joystick::JSB_Y;
		m_btn_map[308] = Joystick::JSB_X;
		m_btn_map[310] = Joystick::JSB_L;
		m_btn_map[311] = Joystick::JSB_R;
		m_btn_map[317] = Joystick::JSB_THUMB_L;
		m_btn_map[318] = Joystick::JSB_THUMB_R;
		m_btn_map[278] = Joystick::JSB_SELECT;
		m_btn_map[315] = Joystick::JSB_START;
		m_btn_map[316] = Joystick::JSB_MENU;
#endif
	}

	bool
	LinuxJoystick::handleEvent(struct input_event &event)
	{
		using namespace Event;
		using namespace Input;

		if (event.type == EV_KEY) {
			InputMap::const_iterator l_entry = m_btn_map.find(event.code);
			if (l_entry == m_btn_map.end())
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

			SharedEvent l_event(new JoystickButtonEvent(
			    static_cast<Input::Joystick::Button>(l_entry->second),
			    l_action,
			    m_btn_state,
			    id()));
			EventManager::Instance()->queue(l_event);
		}
		else if (event.type == EV_ABS) {
			InputMap::const_iterator l_entry = m_abs_map.find(event.code);

			if (l_entry == m_abs_map.end())
				return(false);

			struct input_absinfo l_absinfo;
			memset(&l_absinfo, 0, sizeof(l_absinfo));
			ioctl(fd(), EVIOCGABS(event.code), &l_absinfo);

			SharedEvent l_event(new JoystickAxisEvent(
			    static_cast<Input::Joystick::Axis>(l_entry->second),
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

	bool s_initialized = false;

} /****************************************************** Anonymous Namespace */

namespace Input { /****************************************** Input Namespace */
namespace Linux { /*********************************** Input::Linux Namespace */

bool
EventDevice::Initialize(void)
{
	return(s_initialized = true);
}

void
EventDevice::Finalize(void)
{
	s_initialized = false;
}

bool
EventDevice::InitializeKeyboard(void)
{
	/* initialize evdev */
	if (!s_initialized && !Initialize())
		return(false);

	/* TODO(gamaral) process /proc/bus/input/devices */
	
	LinuxKeyboard *l_device = 0;
	
	static const char s_evdev_template[] = "/dev/input/event%u";
	for (unsigned int i = 0; i < 20; ++i) {
		char l_evdev[sizeof(s_evdev_template) + 4];
		snprintf(l_evdev, sizeof(l_evdev), s_evdev_template, i);

		if (0 == l_device)
			l_device = new LinuxKeyboard;

		if (l_device->open(l_evdev))
			s_kbd_devices.push_back(l_device), l_device = 0;
	}
	delete l_device;

	return(s_kbd_devices.size() > 0);
}

void
EventDevice::FinalizeKeyboard(void)
{
	LinuxEventDeviceList::iterator i;
	LinuxEventDeviceList::const_iterator c = s_kbd_devices.end();

	for (i = s_kbd_devices.begin(); i != c; ++i)
		delete *i;

	s_kbd_devices.clear();
}

void
EventDevice::TickKeyboard(void)
{
	LinuxEventDeviceList::iterator i;
	LinuxEventDeviceList::const_iterator c = s_kbd_devices.end();

	for (i = s_kbd_devices.begin(); i != c; ++i)
		(*i)->processEvents();
}

bool
EventDevice::InitializeJoystick(void)
{
	/* initialize evdev */
	if (!s_initialized && !Initialize())
		return(false);

	/* TODO(gamaral) process /proc/bus/input/devices */
	
	LinuxJoystick *l_device = 0;
	
	static const char s_evdev_template[] = "/dev/input/event%u";
	for (unsigned int i = 0; i < 20; ++i) {
		char l_evdev[sizeof(s_evdev_template) + 4];
		snprintf(l_evdev, sizeof(l_evdev), s_evdev_template, i);

		if (0 == l_device)
			l_device = new LinuxJoystick;

		if (l_device->open(l_evdev))
			s_js_devices.push_back(l_device), l_device = 0;
	}
	delete l_device;

	return(s_js_devices.size() > 0);
}

void
EventDevice::FinalizeJoystick(void)
{
	LinuxEventDeviceList::iterator i;
	LinuxEventDeviceList::const_iterator c = s_js_devices.end();

	for (i = s_js_devices.begin(); i != c; ++i)
		delete *i;

	s_js_devices.clear();
}

void
EventDevice::TickJoystick(void)
{
	LinuxEventDeviceList::iterator i;
	LinuxEventDeviceList::const_iterator c = s_js_devices.end();

	for (i = s_js_devices.begin(); i != c; ++i)
		(*i)->processEvents();
}

} /*************************************************** Input::Linux Namespace */
} /********************************************************** Input Namespace */
MARSHMALLOW_NAMESPACE_END

