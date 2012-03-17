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
 * @buffer
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#ifndef CORE_BUFFERIO_H
#define CORE_BUFFERIO_H 1

#include "core/idataio.h"

#include "core/global.h"

MARSHMALLOW_NAMESPACE_BEGIN

namespace Core
{
	template <class T> class Shared;
	template <class T> class Weak;

	/*! @brief Buffer IO Interface */
	class CORE_EXPORT BufferIO : public IDataIO
	{
		char  *m_buffer;
		const char *m_const_buffer;
		long m_cursor;
		size_t m_size;

	public:

		BufferIO(void);
		BufferIO(char *buffer, size_t size);
		BufferIO(const char *buffer, size_t size);
		virtual ~BufferIO(void);

	public: /* virtual */

		VIRTUAL bool open(const Identifier &ignored, DIOMode mode = DIOReadOnly);
		VIRTUAL void close(void);

		VIRTUAL bool isOpen(void) const;

		VIRTUAL size_t read(char *buffer, size_t bsize);
		VIRTUAL size_t write(const char *buffer, size_t bsize);

		VIRTUAL bool seek(long offset, DIOSeek origin);
		VIRTUAL long tell(void) const
		    { return(m_cursor); }

		VIRTUAL UINT64 size(void) const
		    { return(m_size); }
	};
	typedef Shared<BufferIO> SharedBufferIO;
	typedef Weak<BufferIO> WeakBufferIO;

}

MARSHMALLOW_NAMESPACE_END

#endif
