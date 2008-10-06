/**
 * \file TrashModuleImpl.h
 *
 * \since 2006/10/24
 * \author pavel
 */

#pragma once

#include "resource.h"
#include "ModuleImpl.h"
#include "Defines.h"
#include "DocListPane.h"
#include "DocPropsPane.h"
#include "simplesinglton.h"

typedef CModuleImplT
<
ID_TRASH,
IDS_TRASH, 
0,
0, 
IDB_TRASH_SM,
CDocListPane,
CDocPropsPane
>
CTrashModuleImplBase;

class CTrashModuleImpl 
	:	public CTrashModuleImplBase,
		public CSimpleSinglton2<CTrashModuleImpl, Assist::IApplication>
{
	friend class CSimpleSinglton2<CTrashModuleImpl, Assist::IApplication>;
public:
	virtual void NewItem(int nParentId) override {}
	virtual void DeleteItems(int* pBuf, int nSize) override;

protected:
	CTrashModuleImpl(Assist::IApplication* pApplication);
	CTrashModuleImpl(CTrashModuleImpl const& right);
	virtual ~CTrashModuleImpl(void);
};
