/*
 * Copyright (c) 2012-2013, Guillermo A. Amaral B. (gamaral) <g@maral.me>
 * All rights reserved.
 *
 * This file is part of Marshmallow Game Engine.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of the project as a whole.
 */

#include "evdev_map.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"

#include "input/joystick.h"
#include "input/keyboard.h"

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
namespace EVDEV { /**************************** Input::Linux::EVDEV Namespace */
namespace { /********************* Input::Linux::EVDEV::<anonymous> Namespace */

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
ParseKeyboardSym(const char *sym, int event)
{
	int l_mmcode = 0;

	if (EV_KEY == event) {
		l_mmcode = Keyboard::KBK_NONE;

		if (0 == MMSTRCASECMP("BACKSPACE", sym))
			l_mmcode = Keyboard::KBK_BACKSPACE;
		else if (0 == MMSTRCASECMP("TAB", sym))
			l_mmcode = Keyboard::KBK_TAB;
		else if (0 == MMSTRCASECMP("CLEAR", sym))
			l_mmcode = Keyboard::KBK_CLEAR;
		else if (0 == MMSTRCASECMP("RETURN", sym))
			l_mmcode = Keyboard::KBK_RETURN;
		else if (0 == MMSTRCASECMP("ENTER", sym))
			l_mmcode = Keyboard::KBK_ENTER;
		else if (0 == MMSTRCASECMP("PAUSE", sym))
			l_mmcode = Keyboard::KBK_PAUSE;
		else if (0 == MMSTRCASECMP("ESCAPE", sym))
			l_mmcode = Keyboard::KBK_ESCAPE;

		else if (0 == MMSTRCASECMP("SPACE", sym))
			l_mmcode = Keyboard::KBK_SPACE;
		else if (0 == MMSTRCASECMP("COMMA", sym))
			l_mmcode = Keyboard::KBK_COMMA;
		else if (0 == MMSTRCASECMP("HYPHEN", sym))
			l_mmcode = Keyboard::KBK_HYPHEN;
		else if (0 == MMSTRCASECMP("DASH", sym))
			l_mmcode = Keyboard::KBK_DASH;
		else if (0 == MMSTRCASECMP("MINUS", sym))
			l_mmcode = Keyboard::KBK_MINUS;
		else if (0 == MMSTRCASECMP("PERIOD", sym))
			l_mmcode = Keyboard::KBK_PERIOD;
		else if (0 == MMSTRCASECMP("DECIMAL", sym))
			l_mmcode = Keyboard::KBK_DECIMAL;
		else if (0 == MMSTRCASECMP("DOT", sym))
			l_mmcode = Keyboard::KBK_DOT;
		else if (0 == MMSTRCASECMP("SLASH", sym))
			l_mmcode = Keyboard::KBK_SLASH;

		else if (0 == MMSTRCASECMP("0", sym))
			l_mmcode = Keyboard::KBK_0;
		else if (0 == MMSTRCASECMP("1", sym))
			l_mmcode = Keyboard::KBK_1;
		else if (0 == MMSTRCASECMP("2", sym))
			l_mmcode = Keyboard::KBK_2;
		else if (0 == MMSTRCASECMP("3", sym))
			l_mmcode = Keyboard::KBK_3;
		else if (0 == MMSTRCASECMP("4", sym))
			l_mmcode = Keyboard::KBK_4;
		else if (0 == MMSTRCASECMP("5", sym))
			l_mmcode = Keyboard::KBK_5;
		else if (0 == MMSTRCASECMP("6", sym))
			l_mmcode = Keyboard::KBK_6;
		else if (0 == MMSTRCASECMP("7", sym))
			l_mmcode = Keyboard::KBK_7;
		else if (0 == MMSTRCASECMP("8", sym))
			l_mmcode = Keyboard::KBK_8;
		else if (0 == MMSTRCASECMP("9", sym))
			l_mmcode = Keyboard::KBK_9;

		else if (0 == MMSTRCASECMP("SEMICOLON", sym))
			l_mmcode = Keyboard::KBK_SEMICOLON;
		else if (0 == MMSTRCASECMP("EQUAL", sym))
			l_mmcode = Keyboard::KBK_EQUAL;
		else if (0 == MMSTRCASECMP("EQUALS", sym))
			l_mmcode = Keyboard::KBK_EQUALS;
		else if (0 == MMSTRCASECMP("APOSTROPHE", sym))
			l_mmcode = Keyboard::KBK_APOSTROPHE;
		else if (0 == MMSTRCASECMP("QUOTE", sym))
			l_mmcode = Keyboard::KBK_QUOTE;

		else if (0 == MMSTRCASECMP("A", sym))
			l_mmcode = Keyboard::KBK_A;
		else if (0 == MMSTRCASECMP("B", sym))
			l_mmcode = Keyboard::KBK_B;
		else if (0 == MMSTRCASECMP("C", sym))
			l_mmcode = Keyboard::KBK_C;
		else if (0 == MMSTRCASECMP("D", sym))
			l_mmcode = Keyboard::KBK_D;
		else if (0 == MMSTRCASECMP("E", sym))
			l_mmcode = Keyboard::KBK_E;
		else if (0 == MMSTRCASECMP("F", sym))
			l_mmcode = Keyboard::KBK_F;
		else if (0 == MMSTRCASECMP("G", sym))
			l_mmcode = Keyboard::KBK_G;
		else if (0 == MMSTRCASECMP("H", sym))
			l_mmcode = Keyboard::KBK_H;
		else if (0 == MMSTRCASECMP("I", sym))
			l_mmcode = Keyboard::KBK_I;
		else if (0 == MMSTRCASECMP("J", sym))
			l_mmcode = Keyboard::KBK_J;
		else if (0 == MMSTRCASECMP("K", sym))
			l_mmcode = Keyboard::KBK_K;
		else if (0 == MMSTRCASECMP("L", sym))
			l_mmcode = Keyboard::KBK_L;
		else if (0 == MMSTRCASECMP("M", sym))
			l_mmcode = Keyboard::KBK_M;
		else if (0 == MMSTRCASECMP("N", sym))
			l_mmcode = Keyboard::KBK_N;
		else if (0 == MMSTRCASECMP("O", sym))
			l_mmcode = Keyboard::KBK_O;
		else if (0 == MMSTRCASECMP("P", sym))
			l_mmcode = Keyboard::KBK_P;
		else if (0 == MMSTRCASECMP("Q", sym))
			l_mmcode = Keyboard::KBK_Q;
		else if (0 == MMSTRCASECMP("R", sym))
			l_mmcode = Keyboard::KBK_R;
		else if (0 == MMSTRCASECMP("S", sym))
			l_mmcode = Keyboard::KBK_S;
		else if (0 == MMSTRCASECMP("T", sym))
			l_mmcode = Keyboard::KBK_T;
		else if (0 == MMSTRCASECMP("U", sym))
			l_mmcode = Keyboard::KBK_U;
		else if (0 == MMSTRCASECMP("V", sym))
			l_mmcode = Keyboard::KBK_V;
		else if (0 == MMSTRCASECMP("W", sym))
			l_mmcode = Keyboard::KBK_W;
		else if (0 == MMSTRCASECMP("X", sym))
			l_mmcode = Keyboard::KBK_X;
		else if (0 == MMSTRCASECMP("Y", sym))
			l_mmcode = Keyboard::KBK_Y;
		else if (0 == MMSTRCASECMP("Z", sym))
			l_mmcode = Keyboard::KBK_Z;

		else if (0 == MMSTRCASECMP("BRACKET_L", sym))
			l_mmcode = Keyboard::KBK_BRACKET_L;
		else if (0 == MMSTRCASECMP("BACKSLASH", sym))
			l_mmcode = Keyboard::KBK_BACKSLASH;
		else if (0 == MMSTRCASECMP("BRACKET_R", sym))
			l_mmcode = Keyboard::KBK_BRACKET_R;
		else if (0 == MMSTRCASECMP("GRAVE", sym))
			l_mmcode = Keyboard::KBK_GRAVE;

		else if (0 == MMSTRCASECMP("DELETE", sym))
			l_mmcode = Keyboard::KBK_DELETE;

		else if (0 == MMSTRCASECMP("SHIFT_L", sym))
			l_mmcode = Keyboard::KBK_SHIFT_L;
		else if (0 == MMSTRCASECMP("SHIFT_R", sym))
			l_mmcode = Keyboard::KBK_SHIFT_R;
		else if (0 == MMSTRCASECMP("CAPS_LOCK", sym))
			l_mmcode = Keyboard::KBK_CAPS_LOCK;


		else if (0 == MMSTRCASECMP("ALT_L", sym))
			l_mmcode = Keyboard::KBK_ALT_L;
		else if (0 == MMSTRCASECMP("ALT_R", sym))
			l_mmcode = Keyboard::KBK_ALT_R;

		else if (0 == MMSTRCASECMP("CONTROL_L", sym))
			l_mmcode = Keyboard::KBK_CONTROL_L;
		else if (0 == MMSTRCASECMP("CONTROL_R", sym))
			l_mmcode = Keyboard::KBK_CONTROL_R;

		else if (0 == MMSTRCASECMP("META_L", sym))
			l_mmcode = Keyboard::KBK_META_L;
		else if (0 == MMSTRCASECMP("META_R", sym))
			l_mmcode = Keyboard::KBK_META_R;

		else if (0 == MMSTRCASECMP("SUPER_L", sym))
			l_mmcode = Keyboard::KBK_SUPER_L;
		else if (0 == MMSTRCASECMP("SUPER_R", sym))
			l_mmcode = Keyboard::KBK_SUPER_R;

		else if (0 == MMSTRCASECMP("F1", sym))
			l_mmcode = Keyboard::KBK_F1;
		else if (0 == MMSTRCASECMP("F2", sym))
			l_mmcode = Keyboard::KBK_F2;
		else if (0 == MMSTRCASECMP("F3", sym))
			l_mmcode = Keyboard::KBK_F3;
		else if (0 == MMSTRCASECMP("F4", sym))
			l_mmcode = Keyboard::KBK_F4;
		else if (0 == MMSTRCASECMP("F5", sym))
			l_mmcode = Keyboard::KBK_F5;
		else if (0 == MMSTRCASECMP("F6", sym))
			l_mmcode = Keyboard::KBK_F6;
		else if (0 == MMSTRCASECMP("F7", sym))
			l_mmcode = Keyboard::KBK_F7;
		else if (0 == MMSTRCASECMP("F8", sym))
			l_mmcode = Keyboard::KBK_F8;
		else if (0 == MMSTRCASECMP("F9", sym))
			l_mmcode = Keyboard::KBK_F9;
		else if (0 == MMSTRCASECMP("F10", sym))
			l_mmcode = Keyboard::KBK_F10;
		else if (0 == MMSTRCASECMP("F11", sym))
			l_mmcode = Keyboard::KBK_F11;
		else if (0 == MMSTRCASECMP("F12", sym))
			l_mmcode = Keyboard::KBK_F12;
		else if (0 == MMSTRCASECMP("F13", sym))
			l_mmcode = Keyboard::KBK_F13;
		else if (0 == MMSTRCASECMP("F14", sym))
			l_mmcode = Keyboard::KBK_F14;
		else if (0 == MMSTRCASECMP("F15", sym))
			l_mmcode = Keyboard::KBK_F15;

		else if (0 == MMSTRCASECMP("PRINT", sym))
			l_mmcode = Keyboard::KBK_PRINT;
		else if (0 == MMSTRCASECMP("SCROLL_LOCK", sym))
			l_mmcode = Keyboard::KBK_SCROLL_LOCK;
		else if (0 == MMSTRCASECMP("BREAK", sym))
			l_mmcode = Keyboard::KBK_BREAK;

		else if (0 == MMSTRCASECMP("INSERT", sym))
			l_mmcode = Keyboard::KBK_INSERT;
		else if (0 == MMSTRCASECMP("HOME", sym))
			l_mmcode = Keyboard::KBK_HOME;
		else if (0 == MMSTRCASECMP("END", sym))
			l_mmcode = Keyboard::KBK_END;
		else if (0 == MMSTRCASECMP("PAGE_UP", sym))
			l_mmcode = Keyboard::KBK_PAGE_UP;
		else if (0 == MMSTRCASECMP("PAGE_DOWN", sym))
			l_mmcode = Keyboard::KBK_PAGE_DOWN;

		else if (0 == MMSTRCASECMP("UP", sym))
			l_mmcode = Keyboard::KBK_UP;
		else if (0 == MMSTRCASECMP("DOWN", sym))
			l_mmcode = Keyboard::KBK_DOWN;
		else if (0 == MMSTRCASECMP("LEFT", sym))
			l_mmcode = Keyboard::KBK_LEFT;
		else if (0 == MMSTRCASECMP("RIGHT", sym))
			l_mmcode = Keyboard::KBK_RIGHT;

		else if (0 == MMSTRCASECMP("NUM_LOCK", sym))
			l_mmcode = Keyboard::KBK_NUM_LOCK;
		else if (0 == MMSTRCASECMP("KDIVIDE", sym))
			l_mmcode = Keyboard::KBK_KDIVIDE;
		else if (0 == MMSTRCASECMP("KSLASH", sym))
			l_mmcode = Keyboard::KBK_KSLASH;
		else if (0 == MMSTRCASECMP("KMULTIPLY", sym))
			l_mmcode = Keyboard::KBK_KMULTIPLY;
		else if (0 == MMSTRCASECMP("KASTERISK", sym))
			l_mmcode = Keyboard::KBK_KASTERISK;
		else if (0 == MMSTRCASECMP("KSUBTRACT", sym))
			l_mmcode = Keyboard::KBK_KSUBTRACT;
		else if (0 == MMSTRCASECMP("KDASH", sym))
			l_mmcode = Keyboard::KBK_KDASH;
		else if (0 == MMSTRCASECMP("KHYPHEN", sym))
			l_mmcode = Keyboard::KBK_KHYPHEN;
		else if (0 == MMSTRCASECMP("KMINUS", sym))
			l_mmcode = Keyboard::KBK_KMINUS;
		else if (0 == MMSTRCASECMP("K7", sym))
			l_mmcode = Keyboard::KBK_K7;
		else if (0 == MMSTRCASECMP("K8", sym))
			l_mmcode = Keyboard::KBK_K8;
		else if (0 == MMSTRCASECMP("K9", sym))
			l_mmcode = Keyboard::KBK_K9;
		else if (0 == MMSTRCASECMP("K4", sym))
			l_mmcode = Keyboard::KBK_K4;
		else if (0 == MMSTRCASECMP("K5", sym))
			l_mmcode = Keyboard::KBK_K5;
		else if (0 == MMSTRCASECMP("K6", sym))
			l_mmcode = Keyboard::KBK_K6;
		else if (0 == MMSTRCASECMP("KADD", sym))
			l_mmcode = Keyboard::KBK_KADD;
		else if (0 == MMSTRCASECMP("KPLUS", sym))
			l_mmcode = Keyboard::KBK_KPLUS;
		else if (0 == MMSTRCASECMP("K1", sym))
			l_mmcode = Keyboard::KBK_K1;
		else if (0 == MMSTRCASECMP("K2", sym))
			l_mmcode = Keyboard::KBK_K2;
		else if (0 == MMSTRCASECMP("K3", sym))
			l_mmcode = Keyboard::KBK_K3;
		else if (0 == MMSTRCASECMP("K0", sym))
			l_mmcode = Keyboard::KBK_K0;
		else if (0 == MMSTRCASECMP("KDECIMAL", sym))
			l_mmcode = Keyboard::KBK_KDECIMAL;
		else if (0 == MMSTRCASECMP("KPERIOD", sym))
			l_mmcode = Keyboard::KBK_KPERIOD;
		else if (0 == MMSTRCASECMP("KDOT", sym))
			l_mmcode = Keyboard::KBK_KDOT;
		else if (0 == MMSTRCASECMP("KENTER", sym))
			l_mmcode = Keyboard::KBK_KENTER;

		else if (0 == MMSTRCASECMP("CLOSE", sym))
			l_mmcode = Keyboard::KBK_CLOSE;
		else if (0 == MMSTRCASECMP("HELP", sym))
			l_mmcode = Keyboard::KBK_HELP;
		else if (0 == MMSTRCASECMP("MENU", sym))
			l_mmcode = Keyboard::KBK_MENU;
	}

	return(l_mmcode);
}

inline int
ParseMouseSym(const char * /*sym*/, int /* event */)
{
	/*
	 * TODO(gamaral) Parse mouse syms
	 */
	return(0);
}

} /************************* Input::Linux::EVDEV::<anonymous> Namespace */

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
Map::PopulateEventCodes(uint16_t vendor, uint16_t product, const char *name,
                        Type type, int event, EventCodes &codes, EventAttribute *fuzz)
{
	using namespace TinyXML;

	/* map check */
	if (!s_map_document) return(false);

	/* device type */
	const char *l_entry_type = 0;
	switch (type) {
	case Gamepad:  l_entry_type = "gamepad"; break;
	case Joystick: l_entry_type = "joystick"; break;
	case Keyboard: l_entry_type = "keyboard"; break;
	case Mouse:    l_entry_type = "mouse"; break;

	case Any:
	case Unknown: return(false);
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
#define EVDEV_MAP_REVISION 1
	if (l_root->UnsignedAttribute("revision") != EVDEV_MAP_REVISION) {
		MMERROR("Invalid evdev map file revision: Expecting revision " << EVDEV_MAP_REVISION);
		return(false);
	}

	XMLElement *l_entry = l_root->FirstChildElement(l_entry_type);
	while (l_entry) {
		XMLElement *l_device = l_entry->FirstChildElement("device");
		while (l_device) {
			if (0 == vendor && 0 == product) {
				const char *l_devname = l_device->GetText();
				if (l_devname && 0 == strncmp(name, l_devname, strlen(l_devname)))
					break;
			}
			else {
				unsigned int l_vendor = 0;
				unsigned int l_product = 0;

				if (XML_NO_ERROR ==
					l_device->QueryUnsignedAttribute("vendor", &l_vendor)
				    && l_vendor == vendor
				    && XML_NO_ERROR ==
					l_device->QueryUnsignedAttribute("product", &l_product)
				    && l_product == product)
					break;
			}

			l_device = l_device->NextSiblingElement("device");
		}

		/* check to see if we found the device */
		if (l_device) break;

		l_entry = l_entry->NextSiblingElement(l_entry_type);
	}

	/* exit if we didn't an entry for the device */
	if (!l_entry) return(false);

	/*
	 *  process device entry
	 *
	 */
	XMLElement *l_element = l_entry->FirstChildElement(l_event);
	while(l_element) {
		unsigned int l_code;
		const char *l_sym = l_element->Attribute("sym");

		if (l_sym
		    && XML_NO_ERROR == l_element->QueryUnsignedAttribute("code", &l_code)
		    && static_cast<unsigned short>(l_code) == l_code) {
			/* get axis fuzz */
			if (fuzz) {
				(*fuzz)[static_cast<unsigned short>(l_code)] =
				    l_element->IntAttribute( "fuzz" );
			}

			if (Gamepad == type)
				codes[static_cast<unsigned short>(l_code)] =
				    ParseJoystickSym(l_sym, event);
			else if (Joystick == type)
				codes[static_cast<unsigned short>(l_code)] =
				    ParseJoystickSym(l_sym, event);
			else if (Keyboard == type)
				codes[static_cast<unsigned short>(l_code)] =
				    ParseKeyboardSym(l_sym, event);
			else if (Mouse == type)
				codes[static_cast<unsigned short>(l_code)] =
				    ParseMouseSym(l_sym, event);
		}

		l_element = l_element->NextSiblingElement(l_event);
	}

	return(true);
}

} /******************************************** Input::Linux::EVDEV Namespace */
} /*************************************************** Input::Linux Namespace */
} /********************************************************** Input Namespace */
MARSHMALLOW_NAMESPACE_END

