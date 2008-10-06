/**
 * \file TrashModuleImpl.cpp
 *
 * \since 2006/10/24
 * \author pavel
 */

#include "StdAfx.h"
#include "TrashModuleImpl.h"
#include "ApplicationImpl.h"

CTrashModuleImpl::CTrashModuleImpl(Assist::IApplication* pApplication)
:	CTrashModuleImplBase(pApplication)
{
}

CTrashModuleImpl::CTrashModuleImpl( CTrashModuleImpl const& right )
:	CTrashModuleImplBase(NULL)
{
	assert(false);
}
CTrashModuleImpl::~CTrashModuleImpl(void)
{
}


void CTrashModuleImpl::DeleteItems(int* pBuf, int nSize)
{
	CApplicationImpl::Get().RemoveItems(pBuf, nSize, true, true);
}