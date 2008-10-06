/**
 * @file DocListPane.cpp
 *
 * Notes list view window implementation.
 *
 * @author Pavel Mosunov
 * @since 2004-09-02
 */

#include "stdafx.h"
#include "resource.h"
#include "DocListPane.h"
#include "ApplicationImpl.h"
#include "ModuleLoader.h"
#include "resutils.h"
#include "TrashModuleImpl.h"
#include "dateutils.h"
#include "Items.h"

#define ID_LISTCTRL 100

//////////////////////////////////////////////////////////////////////////
// local functions

CString GetDocListOptionsPath(DWORD mod_id)
{
	TCHAR szPath[MAX_PATH];
	CApplicationImpl::Get().GetModuleOptionsPath(szPath, mod_id);
	return CString(szPath) + CString(_T("\\DocList"));
}


//////////////////////////////////////////////////////////////////////////
//

CDocListPane::CDocListPane() 
:	Views::CListCtrlWindow(GetDocListOptionsPath(CTrashModuleImpl::Get().GetId()), &CTrashModuleImpl::Get())
{

}

CDocListPane::~CDocListPane()
{

}

/**
 * 
 */
void CDocListPane::AddColumns()
{
	m_listCtrl.AddColumn(RESSTR(IDS_NAME), 0);
	m_listCtrl.AddColumn(RESSTR(IDS_PATH), 1);
	m_listCtrl.AddColumn(RESSTR(IDS_DATE_DELETED), 2);
}

/**
 
 */
void CDocListPane::AddBitmaps()
{
	// add modules bitmaps
	IModuleList* pModules = CModuleLoader::Get().GetModules();
	if (pModules->first())
	{
		do 
		{
			CModule& module = pModules->current();

			HBITMAP bm;
			bm = ::LoadBitmap(module.hModule, module.pModuleInfo->GetSmallBitmapRes());
			m_mapModuleImages[module.pModuleInfo->GetId()] = m_listCtrl.GetImageList(LVSIL_SMALL).Add(bm, RGB(192, 192, 192));
			::DeleteObject(bm);

		} while (pModules->next());
	}
}

/**
* 
*/
void CDocListPane::Refresh(int code, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{

	switch (code/*wParam*/)
	{
	case UNM_FOLDER_CHANGED:
		{
			// store selection
			std::map<DWORD, UINT> mapIdToState;
			for (int i = 0; i < m_listCtrl.GetItemCount(); ++i)
			{
				UINT nSelected = m_listCtrl.GetItemState(i, LVIS_SELECTED | LVIS_FOCUSED);
				if (((nSelected & LVIS_SELECTED) == LVIS_SELECTED) ||
					((nSelected & LVIS_FOCUSED) == LVIS_FOCUSED))
				{
					mapIdToState[(DWORD)m_listCtrl.GetItemData(i)] = nSelected;
				}
			}

			// populate list control

			m_listCtrl.DeleteAllItems();
			Assist::IItemIteratorPtr item = CItems::SelectTrash();
			while (item->exec())
			{
				int nItemPos = m_listCtrl.AddItem(m_listCtrl.GetItemCount(), 0, item->GetName(), m_mapModuleImages[item->GetModuleId()]);
				m_listCtrl.SetItemText(nItemPos, 1, CApplicationImpl::Get().GetItemPath(item->GetId()));
				m_listCtrl.SetItemText(nItemPos, 2, dateutils::ToString(item->GetModifiedDate()).c_str());
				m_listCtrl.SetItemData(nItemPos, item->GetId());

				// restore selection
				std::map<DWORD, UINT>::iterator it = mapIdToState.find(item->GetId());
				if (it != mapIdToState.end())
					m_listCtrl.SetItemState(nItemPos, it->second, LVIS_SELECTED | LVIS_FOCUSED);
			}
		}
		break;

	case UNM_ITEM_DELETED:
		for (int i = 0; i < m_listCtrl.GetItemCount(); ++i)
		{
			if (m_listCtrl.GetItemData(i) == wParam)
			{
				m_listCtrl.DeleteItem(i);
			}
		}
		break;
	}
}
