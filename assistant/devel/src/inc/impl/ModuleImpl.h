/*!
 *	\file ModuleInitializer.h
 *
 *	\author Pavel Mosunov 
 *  \since 2005/10/04
 */
#pragma  once

#include <vector>
#include "ModuleInfo.h"
#include "ContainerImpl.h"
#include "AssistViewImpl.h"
//#include "Application.h"

//////////////////////////////////////////////////////////////////////////
// CModuleImplT

template <	int nId,
			int nNameId, 
			int nItemNameId,
			int nBitmapId, 
			int nBitmapSmallId, 
			typename TListView,
			typename TDetailView
			>
class CModuleImplT : public Module::IModuleInfo
{
public:
	CModuleImplT(Assist::IApplication* pApplication)
		:	m_pApplication(pApplication)
	{
		m_nId = nId;
		m_csName.LoadString(nNameId);
		if (nItemNameId != 0) 
			m_csItemName.LoadString(nItemNameId);
		m_nBitmapId = nBitmapId;
		m_nBitmapSmallId = nBitmapSmallId;
	}

	virtual ~CModuleImplT()
	{

	}

	virtual Views::IAssistView* CreateListViewObject() override
	{
		Views::CAssistViewImpl* pView = new TListView();
		return pView;
	}

	virtual Views::IAssistView* CreateDetailViewObject() override
	{
		Views::CAssistViewImpl* pView = new TDetailView();
		return pView;
	}

	virtual int GetId() const override 
	{
		return m_nId;
	}
	virtual LPCTSTR GetName() const override 
	{
		return m_csName;
	}

	virtual LPCTSTR GetItemName() const override
	{
		return m_csItemName;
	}

	LPTSTR GetBitmapRes() const
	{
		if (!m_nBitmapId)
			return NULL;

		return MAKEINTRESOURCE(m_nBitmapId);
	}

	LPTSTR GetSmallBitmapRes() const
	{
		return MAKEINTRESOURCE(m_nBitmapSmallId);
	}

	virtual void CheckStorage(Assist::IDocStorage* pStorageDef) override
	{
 		switch (pStorageDef->GetStorageVer())
 		{
 		case 0: // storage is not exist
 			pStorageDef->Update(NULL, 0x100);
		}
	}

	virtual void DeleteItems(int* pBuf, int nSize) override
	{
		GetMainApp()->DeleteItems(pBuf, nSize, true);
	}

	virtual void NewItem(int nParentId)
	{

	}

	virtual void OpenItem(int nId)
	{

	}

	virtual void OnNewItem(int nItemId, int nParentId) override
	{

	}

	virtual void OnDeleteItem(int nId) override
	{

	}

	Assist::IApplication* GetMainApp() { return m_pApplication; }

protected:

private:
	int m_nId;
	CString m_csName;
	CString m_csItemName;
	int m_nBitmapId;
	int m_nBitmapSmallId;
	safe_ptr<Assist::IApplication> m_pApplication;
	
};

