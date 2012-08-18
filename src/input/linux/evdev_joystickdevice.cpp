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

#include "evdev_joystickdevice.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"

#include "event/eventmanager.h"
#include "event/joystickaxisevent.h"
#include "event/joystickbuttonevent.h"

#include <linux/input.h>

#include <cstring>

#include "../joystick_p.h"

/*
 * EVDEV JoystickDevice Notes
 */

MARSHMALLOW_NAMESPACE_BEGIN
namespace Input { /****************************************** Input Namespace */
namespace Linux { /*********************************** Input::Linux Namespace */
namespace EVDEV { /**************************** Input::Linux::EVDEV Namespace */

JoystickDevice::JoystickDevice(int fd_, Type type_, uint16_t vendor,
    uint16_t product)
    : EventDevice(fd_, type_)
    , m_btn_state(0)
{
	MMDEBUG("Joystick slot id " << id() << " assigned.");

	Map::PopulateEventCodes(vendor, product, name(),
	    type(), EV_ABS, m_abs_map);
	Map::PopulateEventCodes(vendor, product, name(),
	    type(), EV_KEY, m_key_map);
}

bool
JoystickDevice::handleEvent(struct input_event &event)
{
	using namespace Event;

	if (event.type == EV_KEY) {
		Map::EventCodes::const_iterator l_entry =
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
		Map::EventCodes::const_iterator l_entry =
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

} /******************************************** Input::Linux::EVDEV Namespace */
} /*************************************************** Input::Linux Namespace */
} /********************************************************** Input Namespace */
MARSHMALLOW_NAMESPACE_END

