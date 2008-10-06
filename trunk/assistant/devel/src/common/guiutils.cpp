/**
 * @file guiutils.cpp
 *
 * guiutils implementation.
 *
 * @author Pavel Mosunov
 * @since 2004-9-22
 */

#include "StdAfx.h"
#include "guiutils.h"

/**/
int GuiUtils::ErrorMessage(LPCTSTR msg, HWND hWnd /*= NULL*/)
{
	return AtlMessageBox(hWnd, msg, RESSTR(IDR_MAINFRAME), MB_OK | MB_ICONERROR);
}

/**/
BOOL GuiUtils::WinPosIsValid(const CRect& WinPos)
{
	CRect rc;
	::GetWindowRect(GetDesktopWindow(), &rc);
	return rc.PtInRect(WinPos.TopLeft()) && rc.PtInRect(WinPos.BottomRight());
}

/**/
BOOL GuiUtils::GetIconSize(HICON icon, CSize& size)
{
	ICONINFO IconInfo;
	if (!::GetIconInfo(icon, &IconInfo))
		return FALSE;

	size.cx = (short)IconInfo.xHotspot * 2;
	size.cy = (short)IconInfo.yHotspot * 2;

	::DeleteObject(IconInfo.hbmMask);
	::DeleteObject(IconInfo.hbmColor);

	return TRUE;
}

/**/
CString GuiUtils::GetTooltip(UINT nID)
{
	CString s;
	s.LoadString(nID);
	int nRetPos = s.Find('\n');
	if (nRetPos != -1)
	{
		s.Delete(0, ++nRetPos);
	}
	return s;
}

/**/
_tstring GuiUtils::GetSelectedTreeItemPath(CTreeViewCtrl& tree)
{
	_tstring sPath, sItem;
	HTREEITEM ti = tree.GetSelectedItem();

	if (ti)
	{
		sPath.reserve(512);
		sItem.resize(256);
		tree.GetItemText(ti, &sItem[0], 255);
		sPath = _tstring(_T("")) + sItem.c_str();
		HTREEITEM ti_parent = tree.GetParentItem(ti);

		while (ti_parent)
		{
			tree.GetItemText(ti_parent, &sItem[0], 255);
			sPath = sItem.c_str() + _tstring(_T("\\")) + sPath.c_str();
			ti_parent = tree.GetParentItem(ti_parent);
		}
	}
	return sPath;
}

/**/
void GuiUtils::GetSelectedTreeItemPath(CTreeViewCtrl& tree, OUT std::vector<HTREEITEM>& Ids)
{
	HTREEITEM ti = tree.GetSelectedItem();

	Ids.clear();
	Ids.push_back(ti);
	HTREEITEM ti_parent = tree.GetParentItem(ti);
	while (ti_parent)
	{
		Ids.push_back(ti_parent);
		ti_parent = tree.GetParentItem(ti_parent);
	}
}

/**/
BOOL GuiUtils::SelectListViewItem(CListViewCtrl& listCtrl, int nItemPos)
{
	// clear all selections
	for (int i = 0; i < listCtrl.GetItemCount(); ++i)
	{
		listCtrl.SetItemState(i, 0, LVIS_SELECTED | LVIS_FOCUSED);
	}
	// select new item
	BOOL bRet = listCtrl.SetItemState(nItemPos, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	if(bRet)
	{
		bRet = listCtrl.EnsureVisible(nItemPos, FALSE);
	}
	return bRet;
}

/**/
bool GuiUtils::IsParent(HWND hParent, HWND hChild)
{
	while (hChild)
	{
		hChild = ::GetParent(hChild);
		if (hChild == hParent)
		{
			return true;
		}
	}
	return false;
}

/**/
HWND GuiUtils::GetNextTabstopWindow(HWND hDlg, HWND hCtrl)
{
	HWND hCur = hCtrl;
	while (true)
	{
		HWND hNext = ::GetWindow(hCur, GW_CHILD);
		if (!hNext)
		{
			hNext = ::GetWindow(hCur, GW_HWNDNEXT);

			// if not any next and child - go to parent next sibling

			while (!hNext)
			{
				HWND hParent = ::GetParent(hCur);
				if (!hParent)
				{
					return hCtrl;
				}
				else if (hParent == hDlg)
				{
					hNext = hParent; // start from begin
				}
				else
				{
					hNext = ::GetWindow(hParent, GW_HWNDNEXT);
					hCur = hParent;
				}
			}
		}
		if (hNext == hCtrl)
		{
			return hCtrl;
		}
		if (::IsWindowVisible(hNext))
		{
			DWORD dwStyle = (DWORD)::GetWindowLong(hNext, GWL_STYLE);
			if ((dwStyle & (WS_TABSTOP | WS_CHILD)) == (WS_TABSTOP | WS_CHILD))
			{
				return hNext;
			}
		}
		hCur = hNext;
	}
}

/**/
HWND GetLastChild(HWND hNode)
{
	HWND hChild = NULL;
	do
	{
		hChild = ::GetWindow(hNode, GW_CHILD);
		if (hChild)
		{
			hNode = ::GetWindow(hChild, GW_HWNDLAST);
		}
	} while (hChild);

	return hNode;
}

/**/
HWND GuiUtils::GetPrevTabstopWindow(HWND hDlg, HWND hCtrl)
{
	HWND hCur = hCtrl;
	while (true)
	{
		HWND hPrev = ::GetWindow(hCur, GW_HWNDPREV);
		if (hPrev)
		{
			hPrev = GetLastChild(hPrev);
		}
		if (hPrev == NULL)
		{
			hPrev = ::GetParent(hCur);
			if (hPrev == NULL)
			{
				return hCtrl;
			}
			else if (hPrev == hDlg)
			{
				hPrev = GetLastChild(hDlg);
			}
		}
		if (hPrev == hCtrl)
		{
			return hCtrl;
		}
		if (::IsWindowVisible(hPrev))
		{
			DWORD dwStyle = (DWORD)::GetWindowLong(hPrev, GWL_STYLE);
			if ((dwStyle & (WS_TABSTOP | WS_CHILD)) == (WS_TABSTOP | WS_CHILD))
			{
				return hPrev;
			}
		}

		hCur = hPrev;
	}
}

/**/
bool GuiUtils::IsTabstopProcessing(HWND hDlg, MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_TAB && IsParent(hDlg, pMsg->hwnd))
	{
		HWND hNext = NULL;
		if (::GetAsyncKeyState(VK_SHIFT) < 0)
		{
			hNext = GuiUtils::GetPrevTabstopWindow(hDlg, pMsg->hwnd);

			// if input window has no WS_TABSTOP flag - find again
			DWORD dwStyle = (DWORD)::GetWindowLong(pMsg->hwnd, GWL_STYLE);
			if ((dwStyle & (WS_TABSTOP | WS_CHILD)) != (WS_TABSTOP | WS_CHILD))
			{
				hNext = GuiUtils::GetPrevTabstopWindow(hDlg, hNext);
			}
		}
		else
		{
			hNext = GuiUtils::GetNextTabstopWindow(hDlg, pMsg->hwnd);
		}
		if (hNext != NULL)
		{
			::SetFocus(hNext);
		}
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
// CTreeCtrlEnumerator

/**
 * 
 */
void GuiUtils::CTreeCtrlEnumerator::EnumTree(CTreeViewCtrl& tree)
{
	// select item in the tree 
	HTREEITEM hItem = tree.GetRootItem();

	int level = 0;
	while (hItem)
	{
		if (!OnTreeItem(tree, hItem, level))
			break;

		// child
		HTREEITEM hItemNext = 0;
		if (OnIsTreeItemChildProcessing(tree, hItem, level))
			hItemNext = tree.GetChildItem(hItem);

		if (hItemNext) 
			++level;
		else
			// next sibling
			hItemNext = tree.GetNextSiblingItem(hItem);

		// parent next sibling
		if (!hItemNext)
		{
			HTREEITEM hItemParent = tree.GetParentItem(hItem);
			while (hItemParent) 
			{
				--level;
				hItemNext = tree.GetNextSiblingItem(hItemParent);
				if (hItemNext)
					break;

				hItemParent = tree.GetParentItem(hItemParent);
			}
		}
		hItem = hItemNext;
	}
}

/**/
HTREEITEM GuiUtils::CTreeCtrlEnumerator::FindByData(CTreeViewCtrl& tree, int nData)
{
	class CTreeEnumerator : public GuiUtils::CTreeCtrlEnumerator
	{
	public:
		CTreeEnumerator(int nFolder)
			:	m_nFolder(nFolder), m_hItem(0) {}

		virtual BOOL OnTreeItem(CTreeViewCtrl& tree, HTREEITEM hItem, int level)
		{
			if (tree.GetItemData(hItem) == m_nFolder)
			{
				m_hItem = hItem;
				return FALSE;
			}
			return TRUE;
		}
	
		int m_nFolder;
		HTREEITEM m_hItem;

	} enumerator(nData);

	HTREEITEM hItem =  tree.GetSelectedItem();
	if (hItem && tree.GetItemData(hItem) == nData)
	{
		return hItem;
	}

	enumerator.EnumTree(tree);
	return enumerator.m_hItem;
}

/**/
void GuiUtils::CListCtrlOptions::StoreColumns(CListViewCtrl& listCtrl)
{
	Columns.clear();
	CHeaderCtrl header = listCtrl.GetHeader();
	for (int i = 0; i < header.GetItemCount(); ++i)
	{
		CListCtrlOptions::COLUMN_INFO ci;
		ci.Width = listCtrl.GetColumnWidth(i);
		Columns.push_back(ci);
	}
}

/**/
void GuiUtils::CListCtrlOptions::RestoreColumns(CListViewCtrl& listCtrl)
{
	int i = 0; 
	for (CColumnList::iterator it = Columns.begin(); 
		it != Columns.end(); 
		++it)
	{
		if (!listCtrl.SetColumnWidth(i++, it->Width))
			break;
	}
}

//////////////////////////////////////////////////////////////////////////
// CListCtrlState

GuiUtils::CListCtrlState::CListCtrlState(CListViewCtrl& listCtrl) 
:	m_listCtrl(listCtrl),
	m_nTop(0)
{
	StoreState();
}

/**/
GuiUtils::CListCtrlState::~CListCtrlState()
{
	RestoreState();
}

/**/
void GuiUtils::CListCtrlState::StoreState()
{

}

/**/
void GuiUtils::CListCtrlState::RestoreState()
{

}
