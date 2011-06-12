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

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#pragma once

#ifndef CORE_SHARED_H
#define CORE_SHARED_H 1

#include <cassert>

#include "core/global.h"

MARSHMALLOW_NAMESPACE_BEGIN

namespace Core
{

/********************************************************************* Shared */

	template <class T> class Shared;
	template <class T> class Weak;

	/*! @brief Shared Data Struct */
	struct CORE_EXPORT SharedData {
	    void * ptr;
	    INT16 refs;
	    INT16 wrefs;
	};

	/*! @brief Shared Class */
	template <class T>
	class Shared
	{
		template <class X> friend class Weak;
		SharedData *m_data;

	public:
		Shared(void)
		    : m_data(0) {}
		Shared(T *ptr);
		Shared(SharedData *data)
		    : m_data(data) { if (m_data) ++m_data->refs;}
		Shared(const Shared &copy);
		~Shared(void)
		    { clear(); }

		void clear(void);

		T * raw(void) const
		    { return(m_data ? reinterpret_cast<T *>(m_data->ptr) : 0); }

		template <class U> Shared<U> cast(void) const;
		template <class U> Shared<U> staticCast(void) const;

	public: /* operator */

		operator bool(void) const
		    { return(m_data != 0 && m_data->ptr != 0); }

		operator Weak<T>(void) const
		    { return(Weak<T>(m_data)); }

		T & operator *(void) const
		    { return(*reinterpret_cast<T *>(m_data->ptr)); }

		T * operator ->(void) const
		    { return(reinterpret_cast<T *>(m_data->ptr)); }

		Shared & operator =(const Shared &rhs);

		bool operator ==(const Shared &rhs) const
		    { return(this == &rhs || m_data == rhs.m_data); }
	};

	template <class T>
	Shared<T>::Shared(T *ptr)
	    : m_data(new SharedData)
	{
		assert(ptr);
		m_data->ptr  = ptr;
		m_data->refs = 1;
		m_data->wrefs = 0;
	}

	template <class T>
	Shared<T>::Shared(const Shared &copy)
	    : m_data(copy.m_data)
	{
		if (copy) ++m_data->refs;
	}

	template <class T>
	template <class U> Shared<U>
	Shared<T>::cast(void) const
	{
		return(Shared<U>(m_data));
	}

	template <class T>
	template <class U> Shared<U>
	Shared<T>::staticCast(void) const
	{
		UNUSED(static_cast<U *>(raw()));
		return(Shared<U>(m_data));
	}

	template <class T>
	Shared<T> &
	Shared<T>::operator =(const Shared<T> &rhs)
	{
		if (*this == rhs)
			return(*this);

		clear();

		if (rhs) {
			m_data = rhs.m_data;
			++m_data->refs;
		}

		return(*this);
	}

	template <class T>
	void
	Shared<T>::clear(void)
	{
		if (m_data && --m_data->refs <= 0) {
			T *ptr = reinterpret_cast<T *>(m_data->ptr);
			delete ptr;

			if (m_data->wrefs <= 0)
				delete m_data;
			else
				m_data->ptr = 0;
		}
		m_data = 0;
	}

/*********************************************************************** Weak */

	/*! @brief Weak Class */
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
		    { return(m_data != 0 && m_data->ptr != 0); }

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
	};

	template <class T>
	Weak<T>::Weak(const Weak &copy)
	    : m_data(copy.m_data)
	{
		if (copy) ++m_data->wrefs;
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
		UNUSED(static_cast<U *>(raw()));
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
