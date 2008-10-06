/*!
 *	\file ContainerImpl.h
 *
 *	\author Pavel Mosunov 
 *  \since 2005/10/14
 */

#ifndef __CONTAINERIMPL_H__
#define __CONTAINERIMPL_H__

#include "Container.h"

template <typename T>
class CContainerImplT : public  IContainer<typename T::iterator::value_type>
{
public:
	typedef typename T::iterator iterator;
	typedef typename iterator::value_type value_type;

	virtual BOOL first()
	{
		if (empty())
			return FALSE;
		m_current = m_list.begin();
		return TRUE;
	}

	virtual value_type& current()
	{
		return *m_current;
	}

	virtual BOOL next()
	{
		++m_current;
		return !end();
	}

	virtual BOOL end()
	{
		return m_current == m_list.end();
	}

	virtual BOOL empty()
	{
		return m_list.empty();
	}

	virtual int size()
	{
		return m_list.size();
	}

	T m_list;
	iterator m_current;
};

#endif //__CONTAINERIMPL_H__
