/**
 * \file TaskModuleImpl.cpp
 *
 * \since 2006/07/28
 * \author pavel
 */

#include "StdAfx.h"
#include "resource.h"
#include "TaskModuleImpl.h"
#include "resutils.h"

//static const LPCTSTR szType = _T("2A3637F9-CFD5-4071-BE27-F7696809BCDD");

//////////////////////////////////////////////////////////////////////////

CTaskModuleImpl::CTaskModuleImpl(Assist::IApplication* pApplication)
:	CTaskModuleImplBase(pApplication)
{
}

CTaskModuleImpl::CTaskModuleImpl( CTaskModuleImpl const& right )
:	CTaskModuleImplBase(NULL)
{
}

CTaskModuleImpl::~CTaskModuleImpl()
{
}

void CTaskModuleImpl::CheckStorage(Assist::IDocStorage* pStorageDef)
{
	try
	{
		switch (pStorageDef->GetStorageVer())
		{
		case 0: // storage is not exist
			_tstring script = resutils::reshtml(IDR_UPDATE_SQL_0);
			pStorageDef->Update(script.c_str(), 0x100);
		}
	}
	RAISE()
}

void CTaskModuleImpl::NewItem(int nParentId)
{

}
