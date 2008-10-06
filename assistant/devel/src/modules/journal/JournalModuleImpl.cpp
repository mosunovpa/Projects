/**
 * \file JournalModuleImpl.cpp
 *
 * \since 2006/07/28
 * \author pavel
 */

#include "StdAfx.h"
#include "JournalModuleImpl.h"
#include "resutils.h"
#include "errors.h"

CJournalModuleImpl g_Module;

//static const LPCTSTR szType = _T("C0A88E0F-4201-4815-B7FA-5706C505E814");

CJournalModuleImpl::CJournalModuleImpl() 
:	CJournalModuleImplBase(),
	m_pApplication(NULL)
{
}

CJournalModuleImpl::~CJournalModuleImpl(void)
{
}

void CJournalModuleImpl::CheckStorage(Assist::IDocStorage* pStorageDef)
{
	switch (pStorageDef->GetStorageVer())
	{
	case 0:
		
		break;
// 	default:
// 		ThrowError(RESSTR_FMT(IDS_NOT_VALID_DB_VERSION, nCurVersion).c_str());
	}
}
