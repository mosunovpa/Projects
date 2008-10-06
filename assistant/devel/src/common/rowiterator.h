/**
 * \file rowiterator.h
 *
 * \since 2006/08/18
 * \author pavel
 */
#pragma once

#include "releasable.h"

template <class T>
class CRowIteratorT : public IReleasable
{
public:
	virtual T* next() = 0;
	virtual T* row() = 0;
};