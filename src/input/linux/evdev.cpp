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

#include <sys/inotify.h>

#include <fcntl.h>

#include <cstdio>
#include <cstring>

#include "evdev_eventdevice.h"
#include "evdev_map.h"

/*
 * EVDEV Notes
 *
 * Hot Swapping
 * ------------
 * We currently use two methods to detect if an event device is attached or
 * detached, inotify create/destroy and inotify attrib.
 *
 * The first case is fairly straight forward to understand, the second is a bit
 * tricky, I've noticed that on some systems udev doesn't destroy unused nodes,
 * but it does trigger an attribute change. If the event device in question is
 * open, we check if it's still valid, if not, we close and try to open it
 * again.
 */

MARSHMALLOW_NAMESPACE_BEGIN
namespace Input { /****************************************** Input Namespace */
namespace Linux { /*********************************** Input::Linux Namespace */
namespace { /**************************** Input::Linux::<anonymous> Namespace */

namespace ED = EVDEV;

/*
 * Event devices settings
 *
 * EVDEV_MAX is the maximum event device we will try to open (event0..eventN).
 * EVDEV_NAME_MAX 
 */
#define EVDEV_MAX 16u

int s_initialized  =  0;
int s_inotify_fd   = -1;
int s_watch_fd     = -1;

ED::EventDevice *s_evdev[EVDEV_MAX];

void
ProcessNotificationEvents(void)
{
	/* return on invalid handle */
	if (-1 == s_inotify_fd)
		return;

	static const ssize_t l_event_size(sizeof(struct inotify_event));
	char l_raw[l_event_size + NAME_MAX + 1];
	struct inotify_event *l_event;
	ssize_t l_c, l_i = 0;
	unsigned int l_evdev_id = 0;

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
			if (1 != sscanf(l_event->name, "event%ud", &l_evdev_id)
			    || l_evdev_id >= EVDEV_MAX)
				continue;

			ED::EventDevice *&l_evdev = s_evdev[l_evdev_id];
			if (IN_CREATE == (l_event->mask & IN_CREATE))
				l_evdev = ED::EventDevice::Open(l_evdev_id, s_initialized);
			else if (IN_DELETE == (l_event->mask & IN_DELETE))
				delete l_evdev, l_evdev = 0;
			else if (IN_ATTRIB == (l_event->mask & IN_ATTRIB)) {
				/* skip if valid */
				if (l_evdev && l_evdev->isValid())
					continue;

				delete l_evdev, l_evdev = 0;
				l_evdev = ED::EventDevice::Open(l_evdev_id, s_initialized);
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
		memset(s_evdev, 0, sizeof(s_evdev));

		s_inotify_fd = inotify_init1(O_NONBLOCK);
		if (-1 == s_inotify_fd)
			MMWARNING("INotify init failed.");
		else {
			s_watch_fd = inotify_add_watch(s_inotify_fd, "/dev/input",
			                               IN_CREATE|IN_DELETE|IN_ATTRIB);
			if (-1 == s_watch_fd)
				MMWARNING("INotify watch request failed.");
		}
		ED::Map::Initialize();
	}

	int l_c  = 0;
	for (unsigned int i = 0; i < EVDEV_MAX; ++i) {
		ED::EventDevice *&l_evdev = s_evdev[i];

		if (l_evdev && l_evdev->isValid())
			continue;

		/* increase opened counter on success */
		if ((l_evdev = ED::EventDevice::Open(i, mask)))
			++l_c;
	}

	s_initialized |= mask;

	return(l_c > 0);
}

void
Finalize(int mask)
{
	for (unsigned int i = 0; i < EVDEV_MAX; ++i)
		if (s_evdev[i] && 0 != (mask & s_evdev[i]->type()))
			delete s_evdev[i], s_evdev[i] = 0;

	s_initialized &= ~(mask);

	/* truly finalize if we closed all devices */
	if (0 != s_initialized)
		return;

	if (-1 != s_watch_fd)
		inotify_rm_watch(s_inotify_fd, s_watch_fd);
	if (-1 != s_inotify_fd)
		close(s_inotify_fd);

	ED::Map::Finalize();
}

void
Tick(int mask)
{
	ProcessNotificationEvents();

	for (unsigned int i = 0; i < EVDEV_MAX; ++i) {
		ED::EventDevice *&l_evdev = s_evdev[i];
		if (!l_evdev || 0 == (mask & l_evdev->type()))
			continue;
		l_evdev->processEvents();
	}
}

} /************************************** Input::Linux::<anonymous> Namespace */

#ifdef MARSHMALLOW_EVDEV_KEYBOARD
bool
EVDEV::InitializeKeyboard(void)
{
	return(Initialize(KeyboardType));
}

void
EVDEV::FinalizeKeyboard(void)
{
	Finalize(KeyboardType);
}

void
EVDEV::TickKeyboard(void)
{
	Tick(KeyboardType);
}
#endif

#ifdef MARSHMALLOW_EVDEV_JOYSTICK
bool
EVDEV::InitializeJoystick(void)
{
	return(Initialize(JoystickType|GamepadType));
}

void
EVDEV::FinalizeJoystick(void)
{
	Finalize(JoystickType|GamepadType);
}

void
EVDEV::TickJoystick(void)
{
	Tick(JoystickType|GamepadType);
}
#endif

} /*************************************************** Input::Linux Namespace */
} /********************************************************** Input Namespace */
MARSHMALLOW_NAMESPACE_END

