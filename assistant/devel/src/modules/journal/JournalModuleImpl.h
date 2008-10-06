/**
 * \file JournalModuleImpl.h
 *
 * \since 2006/07/28
 * \author pavel
 */

#ifndef JournalModuleImpl_h__
#define JournalModuleImpl_h__

#pragma once

#include "resource.h"
#include "ModuleImpl.h"
#include "JournalListPane.h"
#include "JournalPropsPane.h"
#include "Application.h"

typedef CModuleImplT
	<2, IDS_JOURNAL, IDB_JOURNAL, IDB_JOURNAL_SM, Views::CJournalListPane, Views::CJournalPropsPane>	
	CJournalModuleImplBase;

class CJournalModuleImpl 
:	public CJournalModuleImplBase
{
public:
	CJournalModuleImpl();
	virtual ~CJournalModuleImpl();

	Assist::IApplication* m_pApplication;

protected:
	virtual void CheckStorage(Assist::IDocStorage* pStorageDef);
};

extern CJournalModuleImpl g_Module;

#endif // JournalModuleImpl_h__
