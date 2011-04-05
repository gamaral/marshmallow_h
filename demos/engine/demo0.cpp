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

#include "event/debuglistener.h"
#include "event/eventbase.h"
#include "event/managerbase.h"
#include "engine/enginebase.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Core;

class DemoEngine : public Engine::EngineBase
{
	int m_stop_timer;
	Event::ManagerBase m_event_manager;
	Event::SharedListenerInterface m_debugListener;

public:
	DemoEngine(void)
	: EngineBase(),
	  m_stop_timer(0),
	  m_debugListener(new Event::DebugListener("log.txt"))
	{
		setEventManager(&m_event_manager);
	}

	VIRTUAL void initialize(void)
	{
		EngineBase::initialize();

		eventManager()->connect(m_debugListener, Event::EventBase::Type);
	}

	VIRTUAL void finalize(void)
	{
		eventManager()->disconnect(m_debugListener, Event::EventBase::Type);
		
		EngineBase::finalize();
	}

	VIRTUAL void second(void)
	{
		Event::SharedEventInterface event(new Event::EventBase);
		Event::SharedEventInterface event2(new Event::EventBase);

		eventManager()->queue(event);
		eventManager()->queue(event2);

		/*
		 * Dequeue all on odd seconds
		 */
		eventManager()->dequeue(event, m_stop_timer % 2);

		if (++m_stop_timer == 10)
			stop();
	}
};

int
main(void)
{
	return(DemoEngine().run());
}

