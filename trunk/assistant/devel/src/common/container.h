/*!
 *	\file Container.h
 *
 *	\author Pavel Mosunov 
 *  \since 2005/10/14
 * Use example:
 *	IContainer* cont = getcontainer();
 *	if (cont->first())
 *	{
 *		do
 *		{
 *			func(cont->current());
 *		} while (cont->next());
 *	}
 */
#ifndef __CONTAINER_H__
#define __CONTAINER_H__

template <typename T>
interface IContainer
{
	virtual BOOL first() = 0;
	virtual T& current() = 0;
	virtual BOOL next() = 0;
	virtual BOOL end() = 0;
	virtual BOOL empty() = 0;
	virtual int size() = 0;
};

#endif // __CONTAINER_H__
