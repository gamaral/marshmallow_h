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

#include "sensor.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"
#include "core/shared.h"

#include "event/eventmanager.h"
#include "event/sensorevent.h"

#define SENSOR_ACCELEROMETER_RATE 25000

/*
 * Sensor Notes
 *
 */

MARSHMALLOW_NAMESPACE_BEGIN
namespace Input { /****************************************** Input Namespace */
namespace BB { /***************************************** Input::BB Namespace */
namespace { /******************************* Input::BB::<anonymous> Namespace */

bool
HandleBasicSensorEvent(int type, bps_event_t *e)
{
	float l_value[3];
	Input::Sensor::Type l_sensor =
		Input::Sensor::Unknown;

	switch(type) {
#ifdef MARSHMALLOW_INPUT_BB_ACCELEROMETER
	case SENSOR_ACCELEROMETER_READING: l_sensor = Input::Sensor::Accelerometer; break;
#endif // MARSHMALLOW_INPUT_BB_ACCELEROMETER
#ifdef MARSHMALLOW_INPUT_BB_GRAVITY
	case SENSOR_GRAVITY_READING: l_sensor = Input::Sensor::Gravity; break;
#endif // MARSHMALLOW_INPUT_BB_GRAVITY
#ifdef MARSHMALLOW_INPUT_BB_GYROSCOPE
	case SENSOR_GYROSCOPE_READING: l_sensor = Input::Sensor::Gyroscope; break;
#endif // MARSHMALLOW_INPUT_BB_GYROSCOPE
#ifdef MARSHMALLOW_INPUT_BB_LINEAR_ACCEL
	case SENSOR_LINEAR_ACCEL_READING: l_sensor = Input::Sensor::LinearAcceleration; break;
#endif // MARSHMALLOW_INPUT_BB_LINEAR_ACCEL
#ifdef MARSHMALLOW_INPUT_BB_MAGNETOMETER
	case SENSOR_MAGNETOMETER_READING: l_sensor = Input::Sensor::Magnetometer; break;
#endif // MARSHMALLOW_INPUT_BB_MAGNETOMETER

	default:
		MMWARNING("Unknown basic sensor event!");
		break;
	}

	if (l_sensor == Input::Sensor::Unknown)
		return(false);

	/*
	 * Get sensor values and queue event
	 */
	sensor_event_get_xyz(e, &l_value[0], &l_value[1], &l_value[2]);
	Event::EventManager::Instance()->
	    queue(new Event::SensorEvent(l_sensor, l_value[0], l_value[1], l_value[2], 0));

	MMDEBUG("Sensor event received: " << l_sensor << ": "
	    << l_value[0] << " " << l_value[1] << " " << l_value[2]);

	return(true);
}

bool
HandleSensorEvents(int type, bps_event_t *e)
{
	switch(type) {
	case SENSOR_ACCELEROMETER_READING:
	case SENSOR_GRAVITY_READING:
	case SENSOR_GYROSCOPE_READING:
	case SENSOR_LINEAR_ACCEL_READING:
	case SENSOR_MAGNETOMETER_READING:
		return(HandleBasicSensorEvent(type, e));

	default:
		MMWARNING("Unknown sensor event!");
		break;
	}
	
	return(false);
}

} /***************************************** Input::BB::<anonymous> Namespace */

bool
Sensor::Initialize(void)
{
#ifdef MARSHMALLOW_INPUT_BB_ACCELEROMETER
	if (sensor_is_supported(SENSOR_TYPE_ACCELEROMETER)) {
		sensor_set_rate(SENSOR_TYPE_ACCELEROMETER, SENSOR_ACCELEROMETER_RATE);
		sensor_set_skip_duplicates(SENSOR_TYPE_ACCELEROMETER, true);
		sensor_request_events(SENSOR_TYPE_ACCELEROMETER);
	}
#endif // MARSHMALLOW_INPUT_BB_ACCELEROMETER

#ifdef MARSHMALLOW_INPUT_BB_MAGNETOMETER
	if (sensor_is_supported(SENSOR_TYPE_MAGNETOMETER)) {
		sensor_set_skip_duplicates(SENSOR_TYPE_MAGNETOMETER, true);
		sensor_request_events(SENSOR_TYPE_MAGNETOMETER);
	}
#endif // MARSHMALLOW_INPUT_BB_MAGNETOMETER

#ifdef MARSHMALLOW_INPUT_BB_GYROSCOPE
	if (sensor_is_supported(SENSOR_TYPE_GYROSCOPE)) {
		sensor_set_skip_duplicates(SENSOR_TYPE_GYROSCOPE, true);
		sensor_request_events(SENSOR_TYPE_GYROSCOPE);
	}
#endif // MARSHMALLOW_INPUT_BB_GYROSCOPE

#ifdef MARSHMALLOW_INPUT_BB_LINEAR_ACCEL
	if (sensor_is_supported(SENSOR_TYPE_LINEAR_ACCEL)) {
		sensor_set_skip_duplicates(SENSOR_TYPE_LINEAR_ACCEL, true);
		sensor_request_events(SENSOR_TYPE_LINEAR_ACCEL);
	}
#endif // MARSHMALLOW_INPUT_BB_LINEAR_ACCEL

#ifdef MARSHMALLOW_INPUT_BB_GRAVITY
	if (sensor_is_supported(SENSOR_TYPE_GRAVITY)) {
		sensor_set_skip_duplicates(SENSOR_TYPE_GRAVITY, true);
		sensor_request_events(SENSOR_TYPE_GRAVITY);
	}
#endif // MARSHMALLOW_INPUT_BB_GRAVITY

	return(true);
}

bool
Sensor::HandleEvent(int type, bps_event_t *e)
{
	return(HandleSensorEvents(type, e));
}

} /****************************************************** Input::BB Namespace */
} /********************************************************** Input Namespace */
MARSHMALLOW_NAMESPACE_END

