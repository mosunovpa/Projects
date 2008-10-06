/**
 * @file NoteListPane.cpp
 *
 * Notes list view window implementation.
 *
 * @author Pavel Mosunov
 * @since 2004-09-02
 */

#include "stdafx.h"
#include "resource.h"
#include "assistres.h"
#include "resutils.h"
#include "Application.h"
#include "NoteListPane.h"
#include "NotesModuleImpl.h"
#include "NoteWnd.h"


//////////////////////////////////////////////////////////////////////////
// local functions

CString GetNodeListOptionsPath(DWORD mod_id)
{
	TCHAR szPath[MAX_PATH];
	CNotesModuleImpl::Get().GetMainApp()->GetModuleOptionsPath(szPath, mod_id);
	return CString(szPath) + CString(_T("\\NoteList"));
}

//////////////////////////////////////////////////////////////////////////
//

namespace Views
{

/**
 
 */
CNoteListPane::CNoteListPane() 
:	CListCtrlWindow(GetNodeListOptionsPath(CNotesModuleImpl::Get().GetId()), &CNotesModuleImpl::Get())
{

}

/**
 * CNoteListPane::PreTranslateMessage
 */
BOOL CNoteListPane::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}

/**
 
 */
void CNoteListPane::AddColumns()
{
	m_listCtrl.AddColumn(RESSTR(IDS_NAME), 0);
}

/**
 
 */
void CNoteListPane::AddBitmaps()
{
	HBITMAP bm;
	bm = ::LoadBitmap(_Module.GetResourceInstance(), CNotesModuleImpl::Get().GetSmallBitmapRes());
	m_listCtrl.GetImageList(LVSIL_SMALL).Add(bm, RGB(192, 192, 192));
	::DeleteObject(bm);
}

/**
 
 */
void CNoteListPane::Refresh(int code /*= 0*/, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/) 
{
	switch (code)
	{
	case UNM_FOLDER_CHANGED:
		{
			m_listCtrl.DeleteAllItems();
			int nFolderId = wParam;
			Assist::IItemIteratorPtr spItems = CNotesModuleImpl::Get().GetMainApp()->GetItems(nFolderId);
			while (spItems->exec())
			{
				int nItemPos = m_listCtrl.AddItem(m_listCtrl.GetItemCount(), 0, spItems->GetName(), 0);
				m_listCtrl.SetItemData(nItemPos, spItems->GetId());
			}
		}
		break;

	case UNM_ITEM_ADDED:
		{
			CAssistViewImpl::SetFocus();
			Assist::IItemIteratorPtr spItem = CNotesModuleImpl::Get().GetMainApp()->GetItem(wParam);
			int nItemPos = m_listCtrl.AddItem(m_listCtrl.GetItemCount(), 0, spItem->GetName(), 0);
			m_listCtrl.SetItemData(nItemPos, wParam);
			GuiUtils::SelectListViewItem(m_listCtrl, nItemPos);
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

} // namespace Views
