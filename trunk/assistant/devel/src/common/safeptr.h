/*!
 * \file SafePtr.h
 * 
 * \author Pavel Mosunov
 * \since 2005-4-18
 */

#ifndef __SAFEPTR_H__
#define __SAFEPTR_H__

template <typename T>
class safe_ptr
{
public:
	safe_ptr(T* ptr = NULL) : m_ptr(ptr)
	{
	}
	safe_ptr(safe_ptr<T>& right) : m_ptr(right.get())
	{
	}

	safe_ptr<T>& operator= (T* ptr)
	{
		m_ptr = ptr;
		return (*this);
	}

	T* operator->()
	{
		if (m_ptr == NULL)
		{
			assert(FALSE);
			throw _texception(_T("NULL pointer!"));
		}
		return get();
	}

	operator T* ()
	{
		if (m_ptr == NULL)
		{
			assert(FALSE);
			throw _texception(_T("NULL pointer!"));
		}
		return get();
	}

	T* get()
	{
		return m_ptr;
	}
	
	BOOL is_null()
	{
		return (m_ptr == NULL);
	}

private:
	T* m_ptr;
};

#endif //__SAFEPTR_H__
