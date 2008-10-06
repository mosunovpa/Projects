/*!
 * \file ModuleInfo.h
 * 
 * \author Pavel Mosunov
 * \since 2005-5-27
 */
#ifndef __MODULEINFO_H__
#define __MODULEINFO_H__

#include "AssistView.h"
#include "Container.h"
#include "Application.h"

typedef IContainer<Views::IAssistView*> IViewList;

//////////////////////////////////////////////////////////////////////////
// IModuleInfo struct

namespace Module
{

class IModuleInfo
{
public:
	virtual Views::IAssistView* CreateListViewObject() = 0;
	virtual Views::IAssistView* CreateDetailViewObject() = 0;
	virtual int GetId() const = 0;
	virtual LPCTSTR GetName() const = 0;
	virtual LPTSTR GetBitmapRes() const = 0;
	virtual LPTSTR GetSmallBitmapRes() const = 0;
	virtual void CheckStorage(Assist::IDocStorage* pStorageDef) = 0;
	virtual LPCTSTR GetItemName() const = 0;
	virtual void NewItem(int nParentId) = 0;
	virtual void OpenItem(int nId) = 0;
	virtual void DeleteItems(int* pBuf, int nSize) = 0;
	virtual void OnNewItem(int nItemId, int nParentId) = 0;
	virtual void OnDeleteItem(int nId) = 0;

};
typedef IModuleInfo* (*LPGETMODULEINFOPROC)(Assist::IApplication*);

} // namespace Module

#endif //__MODULEINFO_H__
