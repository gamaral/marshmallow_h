/*
 * Copyright 2011-2012 Marshmallow Engine. All rights reserved.
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

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#pragma once

#ifndef MARSHMALLOW_CORE_WEAK_H
#define MARSHMALLOW_CORE_WEAK_H 1

#include <cassert>

#include <core/environment.h>
#include <core/global.h>
#include <core/namespace.h>

MARSHMALLOW_NAMESPACE_BEGIN

namespace Core
{
	template <class T> class Shared;

	struct SharedData;

	/*! @brief Weak Pointer Class */
	template <class T>
	class Weak
	{
		template <class X> friend class Shared;
		SharedData *m_data;

	public:
		Weak(void)
		    : m_data(0) {};
		Weak(SharedData *data)
		    : m_data(data) { if (m_data) ++m_data->wrefs;}
		Weak(const Weak &copy);
		~Weak(void)
		    { clear(); }

		void clear(void);

		T * raw(void) const
		    { assert(m_data && m_data->ptr);
		      return(reinterpret_cast<T *>(m_data->ptr)); }

		template <class U> Weak<U> cast(void) const;
		template <class U> Weak<U> staticCast(void) const;

	public: /* operator */

		operator bool(void) const
		    { return(m_data && m_data->ptr); }

		operator Shared<T>(void) const
		    { assert(m_data && m_data->ptr);
		      return(Shared<T>(m_data)); }

		T & operator *(void) const
		    { assert(m_data && m_data->ptr);
		      return(*reinterpret_cast<T *>(m_data->ptr)); }

		T * operator ->(void) const
		    { assert(m_data && m_data->ptr);
		      return(reinterpret_cast<T *>(m_data->ptr)); }

		Weak & operator =(const Weak &rhs);

		bool operator ==(const Weak &rhs) const
		    { return(this == &rhs || m_data == rhs.m_data); }

		bool operator !=(const Weak &rhs) const
		    { return(this != &rhs && m_data != rhs.m_data); }
	};

	template <class T>
	Weak<T>::Weak(const Weak &copy)
	    : m_data(copy.m_data)
	{
		if (m_data) ++m_data->wrefs;
	}

	template <class T>
	template <class U> Weak<U>
	Weak<T>::cast(void) const
	{
		return(Weak<U>(m_data));
	}

	template <class T>
	template <class U> Weak<U>
	Weak<T>::staticCast(void) const
	{
		MMUNUSED(static_cast<U *>(raw()));
		return(Weak<U>(m_data));
	}

	template <class T>
	Weak<T> &
	Weak<T>::operator =(const Weak<T> &rhs)
	{
		if (*this == rhs)
			return(*this);

		clear();

		if (rhs) {
			m_data = rhs.m_data;
			++m_data->wrefs;
		}

		return(*this);
	}

	template <class T>
	void
	Weak<T>::clear(void)
	{
		if (m_data && --m_data->wrefs <= 0 && m_data->ptr == 0)
			delete m_data;
		m_data = 0;
	}
}

MARSHMALLOW_NAMESPACE_END

#endif
