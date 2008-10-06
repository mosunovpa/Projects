/**
 * \file TaskModuleImpl.h
 *
 * \since 2006/07/28
 * \author pavel
 */

#ifndef TaskModuleImpl_h__
#define TaskModuleImpl_h__

#include "resource.h"
#include "ModuleImpl.h"
#include "TaskListPane.h"
#include "TaskPropsPane.h"
#include "Application.h"
#include "simplesinglton.h"

typedef CModuleImplT
	<
	1,
	IDS_TASKS, 
	IDS_ITEM_TASK,
	IDB_TASKS, 
	IDB_TASKS_SM,
	Views::CTaskListPane,
	Views::CTaskPropsPane
	>
	CTaskModuleImplBase;


class CTaskModuleImpl 
	:	public CTaskModuleImplBase, 
		public CSimpleSinglton2<CTaskModuleImpl, Assist::IApplication>
{
	friend class CSimpleSinglton2<CTaskModuleImpl, Assist::IApplication>;
public:
	virtual void NewItem(int nParentId) override;
protected:
	CTaskModuleImpl(Assist::IApplication* pApplication);
	CTaskModuleImpl(CTaskModuleImpl const& right);
	virtual ~CTaskModuleImpl();
	virtual void CheckStorage(Assist::IDocStorage* pStorageDef)  override;
	
};

#endif // TaskModuleImpl_h__
