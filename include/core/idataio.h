/*
 * Copyright (c) 2011-2013, Guillermo A. Amaral B. (gamaral) <g@maral.me>
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

#pragma once

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#ifndef MARSHMALLOW_CORE_IDATAIO_H
#define MARSHMALLOW_CORE_IDATAIO_H 1

#include <core/environment.h>
#include <core/namespace.h>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Core { /******************************************** Core Namespace */

	template <class T> class Shared;
	template <class T> class Weak;

	enum DIOMode
	{
		DIOInvalid   = 0,
		DIOReadOnly  = (1 << 0),
		DIOWriteOnly = (1 << 1),
		DIOAppend    = (1 << 3)|DIOWriteOnly,
		DIOText      = (1 << 4),
		DIOCreate    = DIOWriteOnly,
		DIOTruncate  = DIOWriteOnly,
		DIOReadWrite = DIOReadOnly|DIOWriteOnly
	};

	enum DIOSeek
	{
		DIOSet,
		DIOCurrent,
		DIOEnd
	};

	/*!
	 * @brief DataIO Interface
	 */
	struct MARSHMALLOW_CORE_EXPORT
	IDataIO
	{
		virtual ~IDataIO(void);

		/*!
		 * Open DIO
		 *
		 * @param mode Open mode
		 * @return true on success
		 */
		virtual bool open(DIOMode mode = DIOReadOnly) = 0;

		/*!
		 * Close DIO
		 */
		virtual void close(void) = 0;

		/*!
		 * Return current open mode, DIOInvalid is returned when DIO
		 * is closed
		 *
		 * @return current open mode
		 */
		virtual DIOMode mode(void) const = 0;

		/*!
		 * Check for DIO open state
		 * @return true if DIO is open
		 */
		virtual bool isOpen(void) const = 0;

		/*!
		 * Check for DIO reached End-Of-File.
		 * @return true if DIO is at EOF
		 */
		virtual bool atEOF(void) const = 0;

		virtual size_t read(void *buffer, size_t bsize) const = 0;
		virtual size_t write(const void *buffer, size_t bsize) = 0;

		virtual bool seek(long offset, DIOSeek origin) const = 0;
		virtual long tell(void) const = 0;
	};
	typedef Shared<IDataIO> SharedDataIO;
	typedef Weak<IDataIO> WeakDataIO;

} /*********************************************************** Core Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
