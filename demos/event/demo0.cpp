/*
 * Copyright 2011 Marshmallow Engine. All rights reserved.
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

#include "core/logger.h"
#include "core/shared.h"
#include "core/type.h"
#include "event/debugeventlistener.h"
#include "event/eventbase.h"
#include "event/eventmanager.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Core;

class CustomEvent : public Event::EventBase
{
	public:
		CustomEvent(TIME t)
#define PRIORITY_HIGH 1
		    : Event::EventBase(t, PRIORITY_HIGH)
		    {}

	public: /* virtual */

		VIRTUAL const Core::Type & type(void) const
		    { return(Type); }

	public: /* static */

		static const Core::Type Type;
};
const Core::Type CustomEvent::Type("CustomEvent");

int
main(void)
{
	Event::EventManager event_manager("main");
	CustomEvent event1(NOW());
	Event::SharedEvent event2(new CustomEvent(NOW()+2000));
	Event::SharedEvent event3(new CustomEvent(NOW()+1000));
	Event::SharedEvent event4(new CustomEvent(NOW()-1000));
	Event::SharedEventListener dl(new Event::DebugEventListener("log.txt"));

	event_manager.connect(dl, CustomEvent::Type);
	event_manager.connect(dl, CustomEvent::Type);

	event_manager.queue(event4);
	event_manager.queue(event3);
	event_manager.queue(event2);

	event_manager.execute(0); // SWITCH QUEUE
	while(!event_manager.execute(INFINITE)) {
		INFO1("WAITING FOR FUTURE STAMPED EVENT!");
		Platform::Sleep(100);
	}

	event_manager.dispatch(event1);

	event_manager.disconnect(dl, CustomEvent::Type);
	return(0);
}

