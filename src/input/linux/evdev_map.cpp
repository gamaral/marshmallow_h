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

#include "input/linux/evdev_map.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"

#include "input/joystick.h"

#include <linux/input.h>

#include <fcntl.h>
#include <stdint.h>

#include <cstdio>
#include <cstring>

#include <tinyxml2.h>

/*
 * EVDEV Map Notes
 *
 */

MARSHMALLOW_NAMESPACE_BEGIN
namespace Input { /****************************************** Input Namespace */
namespace Linux { /*********************************** Input::Linux Namespace */
namespace EventDevice { /**************** Input::Linux::EventDevice Namespace */
namespace { /*************** Input::Linux::EventDevice::<Anonymous> Namespace */

namespace TinyXML = tinyxml2;
TinyXML::XMLDocument *s_map_document(0);

inline int
ParseJoystickSym(const char *sym, int event)
{
	int l_mmcode = 0;

	if (EV_ABS == event) {
		l_mmcode = Joystick::JSA_NONE;

		if (0 == MMSTRCASECMP("HX", sym))
			l_mmcode = Joystick::JSA_HX;
		else if (0 == MMSTRCASECMP("HY", sym))
			l_mmcode = Joystick::JSA_HY;
		else if (0 == MMSTRCASECMP("X", sym))
			l_mmcode = Joystick::JSA_X;
		else if (0 == MMSTRCASECMP("Y", sym))
			l_mmcode = Joystick::JSA_Y;
		else if (0 == MMSTRCASECMP("Z", sym))
			l_mmcode = Joystick::JSA_Z;
		else if (0 == MMSTRCASECMP("RX", sym))
			l_mmcode = Joystick::JSA_RX;
		else if (0 == MMSTRCASECMP("RY", sym))
			l_mmcode = Joystick::JSA_RY;
		else if (0 == MMSTRCASECMP("RZ", sym))
			l_mmcode = Joystick::JSA_RZ;
	}
	else if (EV_KEY == event) {
		l_mmcode = Joystick::JSB_NONE;

		if (0 == MMSTRCASECMP("A", sym))
			l_mmcode = Joystick::JSB_A;
		else if (0 == MMSTRCASECMP("B", sym))
			l_mmcode = Joystick::JSB_B;
		else if (0 == MMSTRCASECMP("X", sym))
			l_mmcode = Joystick::JSB_X;
		else if (0 == MMSTRCASECMP("Y", sym))
			l_mmcode = Joystick::JSB_Y;
		else if (0 == MMSTRCASECMP("L", sym))
			l_mmcode = Joystick::JSB_L;
		else if (0 == MMSTRCASECMP("R", sym))
			l_mmcode = Joystick::JSB_R;
		else if (0 == MMSTRCASECMP("THUMB_L", sym))
			l_mmcode = Joystick::JSB_THUMB_L;
		else if (0 == MMSTRCASECMP("THUMB_R", sym))
			l_mmcode = Joystick::JSB_THUMB_R;
		else if (0 == MMSTRCASECMP("START", sym))
			l_mmcode = Joystick::JSB_START;
		else if (0 == MMSTRCASECMP("SELECT", sym))
			l_mmcode = Joystick::JSB_SELECT;
		else if (0 == MMSTRCASECMP("MENU", sym))
			l_mmcode = Joystick::JSB_MENU;
		else if (0 == MMSTRCASECMP("DOWN", sym))
			l_mmcode = Joystick::JSB_DOWN;
		else if (0 == MMSTRCASECMP("LEFT", sym))
			l_mmcode = Joystick::JSB_LEFT;
		else if (0 == MMSTRCASECMP("RIGHT", sym))
			l_mmcode = Joystick::JSB_RIGHT;
		else if (0 == MMSTRCASECMP("UP", sym))
			l_mmcode = Joystick::JSB_UP;
	}

	return(l_mmcode);
}

inline int
ParseKeyboardSym(const char * /*sym*/, int /* event */)
{
	/*
	 * TODO(gamaral) Parse keyboard syms
	 */
	return(0);
}

inline int
ParseMouseSym(const char * /*sym*/, int /* event */)
{
	/*
	 * TODO(gamaral) Parse mouse syms
	 */
	return(0);
}

} /************************* Input::Linux::EventDevice::<Anonymous> Namespace */

bool
Map::Initialize(void)
{
	s_map_document = new TinyXML::XMLDocument;
	if (TinyXML::XML_NO_ERROR != s_map_document->LoadFile("evdev.xml")) {
		MMWARNING("Failed to load the evdev map file.");
		delete s_map_document, s_map_document = 0;
		return(false);
	}

	return(true);
}


void
Map::Finalize(void)
{
	delete s_map_document, s_map_document = 0;
}

bool
Map::PopulateEventCodes(__u16 vendor, __u16 product, const char *name,
                        Type type, int event, EventCodes &codes)
{
	using namespace TinyXML;

	/* map check */
	if (!s_map_document) return(false);

	/* device type */
	const char *l_device_type = 0;
	switch (type) {
	case GamepadType:  l_device_type = "gamepad"; break;
	case JoystickType: l_device_type = "joystick"; break;
	case KeyboardType: l_device_type = "keyboard"; break;
	case MouseType:    l_device_type = "mouse"; break;

	case AnyType:
	case UnknownType: return(false);
	}

	/* event type */
	const char *l_event = 0;
	switch (event) {
	case EV_ABS: l_event = "abs"; break;
	case EV_KEY: l_event = "key"; break;
	case EV_REL: l_event = "rel"; break;

	/* unhandled event type, skip */
	default: return(false);
	}

	/*
	 * search for device entry
	 *
	 */
	XMLElement *l_root = s_map_document->RootElement();
	XMLElement *l_device = l_root->FirstChildElement(l_device_type);
	while (l_device) {
		if (0 == vendor && 0 == product) {
			XMLElement *l_name = l_device->FirstChildElement("name");
			const char *l_devname = l_name ? l_name->GetText() : 0;
			if (l_devname && 0 == strncmp(name, l_devname, strlen(l_devname)))
				break;
		}
		else {
			unsigned int l_vendor;
			unsigned int l_product;

			if (XML_NO_ERROR ==
			        l_device->QueryUnsignedAttribute("vendor", &l_vendor)
			    && l_vendor == vendor
			    && XML_NO_ERROR ==
			        l_device->QueryUnsignedAttribute("product", &l_product)
			    && l_product == product)
				break;
		}

		l_device = l_device->NextSiblingElement(l_device_type);
	}

	/* check to see if we found the device */
	if (!l_device) return(false);

	/*
	 *  process device entry
	 *
	 */
	XMLElement *l_element = l_device->FirstChildElement(l_event);
	while(l_element) {
		unsigned int l_code;
		const char *l_sym = l_element->Attribute("sym");

		if (l_sym
		    && XML_NO_ERROR == l_element->QueryUnsignedAttribute("code", &l_code)
		    && static_cast<unsigned short>(l_code) == l_code) {
			if (GamepadType == type)
				codes[static_cast<unsigned short>(l_code)] =
				    ParseJoystickSym(l_sym, event);
			else if (JoystickType == type)
				codes[static_cast<unsigned short>(l_code)] =
				    ParseJoystickSym(l_sym, event);
			else if (KeyboardType == type)
				codes[static_cast<unsigned short>(l_code)] =
				    ParseKeyboardSym(l_sym, event);
			else if (MouseType == type)
				codes[static_cast<unsigned short>(l_code)] =
				    ParseMouseSym(l_sym, event);
		}

		l_element = l_element->NextSiblingElement(l_event);
	}

	return(true);
}

} /************************************** Input::Linux::EventDevice Namespace */
} /*************************************************** Input::Linux Namespace */
} /********************************************************** Input Namespace */
MARSHMALLOW_NAMESPACE_END

