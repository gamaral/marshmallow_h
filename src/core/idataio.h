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

#pragma once

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#ifndef CORE_IDATAIO_H
#define CORE_IDATAIO_H 1

#include "core/identifier.h"

MARSHMALLOW_NAMESPACE_BEGIN

namespace Core
{
	template <class T> class Shared;
	template <class T> class Weak;

	enum DIOMode
	{
		DIOReadOnly  = 1,
		DIOWriteOnly = 2,
		DIOReadWrite = DIOReadOnly | DIOWriteOnly,
		DIOBinary    = 4
	};

	enum DIOSeek
	{
		DIOStart,
		DIOCurrent,
		DIOEnd
	};

	/*! @brief Data IO Interface */
	struct CORE_EXPORT IDataIO
	{
		virtual ~IDataIO(void) {};

		virtual bool open(const Identifier &id, DIOMode mode = DIOReadOnly) = 0;
		virtual void close(void) = 0;

		virtual bool isOpen(void) const = 0;

		virtual size_t read(char *buffer, size_t bsize) = 0;
		virtual size_t write(const char *buffer, size_t bsize) = 0;

		virtual bool seek(long int offset, DIOSeek origin) = 0;
		virtual long int tell(void) const = 0;

		virtual long int size(void) const = 0;
	};
	typedef Shared<IDataIO> SharedDataIO;
	typedef Weak<IDataIO> WeakDataIO;

}

MARSHMALLOW_NAMESPACE_END

#endif
