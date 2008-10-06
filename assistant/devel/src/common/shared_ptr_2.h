/**
 * \file shared_ptr_2.h
 *
 * \since 2006/07/14
 * \author pavel
 */

#ifndef shared_ptr_2_h__
#define shared_ptr_2_h__

#include <boost/shared_ptr.hpp>

template <class T>
struct deleter
{
	void operator() (T* p)
	{
		if (p) p->release();
	}
};

template <class T>
class shared_ptr_2 : public boost::shared_ptr<T>
{
	deleter<T> d;
public:
	shared_ptr_2(T* p) : boost::shared_ptr<T>(p, d) {}
};

#endif // shared_ptr_2_h__
