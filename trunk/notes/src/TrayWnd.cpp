#include "stdafx.h"
#include "TrayWnd.h"
#include "resutils.h"
#include "Application.h"
#include "menuutils.h"
#include "shlwapi.h"
#include "NewLabelDialog.h"
#include "Clipboard.h"
#include "defines.h"

#define MFT_EX_DELETED 0x00000001L

///////////////////////////////////////////////////////////
// CTrayWnd

/* constructor*/
CTrayWnd::CTrayWnd() : m_nSelectedNoteCmd(0)
{

}

/* destructor */
CTrayWnd::~CTrayWnd()
{
	// Destroy the menu
	if (m_menuPopup.m_hMenu)
		m_menuPopup.DestroyMenu();
}

/**/
CMenuHandle CTrayWnd::GetMenuNotes() const
{
	if (::IsMenu(m_menuPopup.m_hMenu) )
	{
		return m_menuPopup.GetSubMenu(0);
	}
	return 0;
}

/**/
CMenuHandle CTrayWnd::GetDeletedMenu() const
{
	CMenuHandle menuNotes = GetMenuNotes();
	if (menuNotes.m_hMenu)
	{
		int nCount = menuNotes.GetMenuItemCount();
		return menuNotes.GetSubMenu(nCount - 7);
	}
	return 0;
}

/**/
HMENU CTrayWnd::GetTooltipMenu() const
{
	return GetMenuNotes();
}

/**/
_tstring CTrayWnd::GetTooltipText(int nSelectedMenuItemId) const
{
	if (IS_NOTE_CMD(nSelectedMenuItemId))
	{
		return CApplication::Get().GetNoteText(GET_NOTE_ID_FROM_CMD(nSelectedMenuItemId));
	}
	return _tstring();
}

/**/
BOOL CTrayWnd::BeforeTooltipShowing() const
{
	return !::IsMenu(m_menuNoteActions);
}


/* WM_CREATE */
LRESULT CTrayWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
#ifdef COOL_CONTEXT_MENU
	GetSystemSettings();
#endif
	m_Icon.LoadIcon(IDR_MAINFRAME, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(m_Icon, TRUE);
	m_IconSm.LoadIcon(IDR_MAINFRAME, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(m_IconSm, FALSE);

	// Load the 'Notes' icon from the application's resources
	m_hIconNotes = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME),
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	ATLASSERT(m_hIconNotes);

	// Add the icon to the taskbar status area
	_tstring sAppName = resutils::resstring(IDS_APP_NAME);
	if (sAppName.empty() || !m_TrayIcon.AddIconToTaskBar(m_hWnd, IDR_MAINFRAME, m_hIconNotes, &sAppName[0]))
	{
		ATLTRACE(_T("Icon was not added to the taskbar!\n"));
		return 0;
	}

	// adjust system menu
	CMenuHandle menu = GetSystemMenu(FALSE);
	int nCount = menu.GetMenuItemCount();
	for (int i = nCount - 1; i >= 0;  --i)
	{
		int id = menu.GetMenuItemID(i);
		if (id != SC_CLOSE)
		{
			menu.DeleteMenu(id, MF_BYCOMMAND);
		}
	}

	// images for menu icons
	m_ImageList.CreateFromImage(IDB_TRAY_MENU, 16, 1, CLR_DEFAULT, IMAGE_BITMAP, LR_CREATEDIBSECTION | LR_DEFAULTSIZE);

	BOOL bAlwaisOnTop = TRUE; //CApplication::Get().GetOptions().GetAlwaysOnTop();
	SetWindowPos(bAlwaisOnTop ? HWND_TOPMOST : HWND_NOTOPMOST, CRect(0,0,0,0), SWP_NOSIZE | SWP_NOMOVE);

	return 0;
}

/* WM_DESTROY */
LRESULT CTrayWnd::OnDestroy(void)
{
	CApplication::Get().ReleaseStorage();

	// Delete the icon from the taskbar status area
	if (!m_TrayIcon.RemoveIconFromTaskBar(m_hWnd, IDR_MAINFRAME))
		ATLTRACE(_T("Icon was not removed from the taskbar!\n"));

	::PostQuitMessage(0);
	SetMsgHandled(FALSE);

	return 0;
}

/* WM_QUERYENDSESSION */
LRESULT CTrayWnd::OnQueryEndSession(UINT wParam, UINT lParam)
{
	OnDestroy();
	return TRUE;
}

/* WMU_NOTIFYICON */
LRESULT CTrayWnd::OnNotifyIcon(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Determine the tray message
	switch (lParam)
	{
		// Right mouse button click - display a popup menu
	case WM_RBUTTONUP:
		DisplayShortcutMenu();
		break;
	case WM_LBUTTONDBLCLK:
		break;
	case WM_LBUTTONUP:
		::SetForegroundWindow(m_hWnd);
		PostMessage(WM_COMMAND, ID_POPUP_NEWNOTE);
		break;
	default:
		break;
	}
	return 0;
}


/*
WM_SETFOCUS
*/
void CTrayWnd::OnFocus(HWND hWnd)
{
	CApplication::Get().ActivateTopNote();
	SetMsgHandled(FALSE);
}

/*
WM_SYSCOMMAND
*/
void CTrayWnd::OnSysCommand(UINT nID, CPoint pt)
{
	if (nID == SC_CLOSE)
	{
		CApplication::Get().CloseAllNotes();
	}
	else
	{
		SetMsgHandled(FALSE);
	}
}

/**/
LRESULT CTrayWnd::DisplayShortcutMenu()
{
	if (m_menuPopup.IsMenu())
	{
		return 0;
	}
	// Load the menu resource
	if (!m_menuPopup.LoadMenu(IDR_TRAYMENU))
	{
		ATLTRACE(_T("Menu was not loaded!\n"));
		return 0;
	}

	// TrackPopupMenu cannot display the menu bar so get
	// a handle to the first shortcut menu
	CMenuHandle menuTrackPopup = GetMenuNotes();
	if (!menuTrackPopup.m_hMenu)
	{
		ATLTRACE(_T("Handle to the first shortcut menu was not retrieved!\n"));
		m_menuPopup.DestroyMenu();
		m_menuPopup.m_hMenu = NULL;
		return 0;
	}

	// Display the shortcut menu. Track the right mouse button

	// Display a shortcut menu at the specified location
	// See "PRB: Menus for Notification Icons Don't Work Correctly" in MSDN
	::SetForegroundWindow(m_hWnd);

	// Get cursor's position
	POINT pt;
	::GetCursorPos(&pt);
	if (!menuTrackPopup.TrackPopupMenuEx(TPM_RIGHTALIGN | TPM_BOTTOMALIGN /*| TPM_NOANIMATION*/, pt.x, pt.y, m_hWnd, NULL))
	{
		ATLTRACE(_T("Shortcut menu was not displayed!\n"));
		m_menuPopup.DestroyMenu();
		m_menuPopup.m_hMenu = NULL;
		return 0;
	}
	::PostMessage(m_hWnd, WM_NULL, 0, 0);
	
	// Destroy the menu and free any memory that the menu occupies
	m_menuPopup.DestroyMenu();
	m_menuPopup.m_hMenu = NULL;

	return 0;
}

/* WM_INITMENUPOPUP */
void CTrayWnd::OnInitMenuPopup(CMenuHandle menuPopup, UINT nIndex, BOOL bSysMenu)
{
	if (menuPopup.m_hMenu == GetMenuNotes().m_hMenu)
	{
		// set menu item states
		menuPopup.SetMenuDefaultItem(ID_POPUP_NEWNOTE);

		m_listNotesMenuActions.clear();
		ModifyNotesMenu(menuPopup);
	}
//	menuutils::UpdateMenuWindow(menuPopup);
//	PostMessage(WMU_MENUUPDATE, (WPARAM)menuPopup.m_hMenu);
	SetMsgHandled(FALSE);
}

/* WM_UNINITMENUPOPUP */
void CTrayWnd::OnUnInitMenuPopup(UINT nID, CMenuHandle menu)
{
	SetMsgHandled(FALSE);
}

/**/
void CTrayWnd::ProcessCheckedMenu(CNotesMenuItem::Actions action)
{
	_tstring clipboartText;
	_tstring textSeparator = _T("\r\n---\r\n");
	for (CNotesMenuActions::iterator it = m_listNotesMenuActions.begin();
		it != m_listNotesMenuActions.end(); ++it)
	{
		if (it->IsState(CNotesMenuItem::stChecked))
		{
			switch (action)
			{
			case CNotesMenuItem::acOpen:
				CApplication::Get().ShowNote(it->m_nNoteId);
				break;
			case CNotesMenuItem::acDelete:
				CApplication::Get().DeleteNote(it->m_nNoteId);
				break;
			case CNotesMenuItem::acLabel:
				CApplication::Get().SetNoteLabel(it->m_nNoteId, m_listNotesMenuActions.m_sLabel.c_str());
				break;
			case CNotesMenuItem::acClipboard:
				{
					_tstring s = CApplication::Get().GetNoteText(it->m_nNoteId);
					if (!clipboartText.empty()) 
					{
						clipboartText += textSeparator;
					}
					clipboartText += s;
				}
				break;
			}
		}
	}
	if (!clipboartText.empty())
	{
		CClipboard::SetText(clipboartText.c_str(), m_hWnd);
	}
	
	m_listNotesMenuActions.clear();
}

/* ID_POPUP_NEWNOTE */
void CTrayWnd::OnPopupNewnote(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	CApplication::Get().CreateNote();
}

/* ID_POPUP_ABOUT */
void CTrayWnd::OnPopupAbout(UINT uNotifyCode, int nID, CWindow wndCtl)
{
}

/* ID_POPUP_EXIT */
void CTrayWnd::OnPopupExit(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	PostMessage(WM_CLOSE);
}

/* NOTE_CMD_FIRST - NOTE_CMD_LAST */
void CTrayWnd::OnNoteSelected(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	CApplication::Get().ShowNote(GET_NOTE_ID_FROM_CMD(nID));
}

/**/
void CTrayWnd::PopulateLabelMenu(CMenuHandle menuLabels, _tstring const& sLabel, BOOL bCheckPadio /*= TRUE*/)
{
	menuLabels.DeleteMenu(ID_DUMMY, MF_BYCOMMAND);

	m_listLabels.clear();
	CApplication::Get().GetLabels(m_listLabels);

	menuLabels.EnableMenuItem(ID_TNM_CLEARLABEL, sLabel.empty() ? MF_GRAYED : MF_ENABLED);
	if (!m_listLabels.empty())
	{
		menuLabels.AppendMenu(MF_SEPARATOR);
	}

//	menuLabels.AppendMenu(MF_STRING, LABEL_CMD_FIRST, resutils::resstring(IDS_NO_LABEL).c_str());

	int nSelCmd = LABEL_CMD_FIRST;
	int pos = 1;
	for (std::list<_tstring>::iterator it = m_listLabels.begin();
		it != m_listLabels.end(); ++it)
	{
		int nCmd = CREATE_LABEL_CMD(pos);
		menuLabels.AppendMenu(MF_STRING, nCmd, it->c_str());
		if (bCheckPadio && *it == sLabel)
		{
			nSelCmd = nCmd;
		}
		++pos;
	}
	if (bCheckPadio)
	{
		menuLabels.CheckMenuRadioItem(LABEL_CMD_FIRST, LABEL_CMD_LAST, nSelCmd, MF_BYCOMMAND);
	}
	
}

/* WM_MENURBUTTONUP */
void CTrayWnd::OnMenuRButtonUp(WPARAM wParam, CMenuHandle menu)
{
	HideTooltip();
	UINT state  = menu.GetMenuState(wParam, MF_BYPOSITION);
	if ((state & MF_GRAYED == MF_GRAYED) ||
		(::IsMenu(m_menuNoteActions)) ||
		!IS_NOTE_CMD(m_nSelectedNoteCmd))
	{
		return;
	}
 	POINT pt;
 	::GetCursorPos(&pt);

	CNotesMenuActions::iterator it = m_listNotesMenuActions.find(GET_NOTE_ID_FROM_CMD(m_nSelectedNoteCmd));
	if (it != m_listNotesMenuActions.end())
	{
		_tstring sLabel = CApplication::Get().GetNoteLabel(GET_NOTE_ID_FROM_CMD(m_nSelectedNoteCmd));

		m_menuNoteActions.LoadMenu(IDR_TRAY_NOTE_MENU);
		CMenuHandle submenu;

		if (it->IsState(CNotesMenuItem::stChecked))
		{
			submenu = m_menuNoteActions.GetSubMenu(2);
			submenu.SetMenuDefaultItem(ID_TNM_OPEN_NOTE);
			PopulateLabelMenu(submenu.GetSubMenu(2), sLabel, FALSE);
		}
		else if (it->IsState(CNotesMenuItem::stDeleted))
		{
			submenu = m_menuNoteActions.GetSubMenu(1);
			submenu.SetMenuDefaultItem(ID_TNM_OPEN_NOTE);
		}
		else
		{
			submenu = m_menuNoteActions.GetSubMenu(0);
			submenu.SetMenuDefaultItem(ID_TNM_OPEN_NOTE);
			PopulateLabelMenu(submenu.GetSubMenu(2), sLabel);
		}

		submenu.TrackPopupMenuEx(TPM_LEFTALIGN | TPM_RECURSE /*| TPM_NOANIMATION*/, pt.x, pt.y, m_hWnd, NULL);
		m_menuNoteActions.DestroyMenu();
	}

}

/**/
BOOL CTrayWnd::IsMenuState(int id, CNotesMenuItem::States nState) 
{
	CNotesMenuActions::iterator it = m_listNotesMenuActions.find(id);
	if (it != m_listNotesMenuActions.end())
	{
		if (it->IsState(nState))
		{
			return TRUE;
		}
	}
	return FALSE;
}

/* ID_TNM_OPEN_NOTE */
void CTrayWnd::OnOpenNote(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if (IS_NOTE_CMD(m_nSelectedNoteCmd))
	{
		if (IsMenuState(GET_NOTE_ID_FROM_CMD(m_nSelectedNoteCmd), CNotesMenuItem::stChecked))
		{
			ProcessCheckedMenu(CNotesMenuItem::acOpen);
		}
		else
		{
			CApplication::Get().ShowNote(GET_NOTE_ID_FROM_CMD(m_nSelectedNoteCmd));
		}
		EndMenu();
	}
}

/* ID_TNM_COPYALLTOCLIPBOARD */
void CTrayWnd::OnCopyAllToClipboard(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if (IS_NOTE_CMD(m_nSelectedNoteCmd))
	{
		if (IsMenuState(GET_NOTE_ID_FROM_CMD(m_nSelectedNoteCmd), CNotesMenuItem::stChecked))
		{
			ProcessCheckedMenu(CNotesMenuItem::acClipboard);
		}
		else
		{
			CApplication::Get().NoteTextToClipboard(GET_NOTE_ID_FROM_CMD(m_nSelectedNoteCmd));
		}
		EndMenu();
	}
}

/* WMU_NEW_LABEL */
LRESULT CTrayWnd::OnWMUNewLabel(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CNewLabelDialog	dlg;
	dlg.m_nInitParam = (CNewLabelDialog::ipCursorPos | CNewLabelDialog::ipPopup);
	UINT nId = GET_NOTE_ID_FROM_CMD(wParam);

	if (!IsMenuState(nId, CNotesMenuItem::stChecked))
	{
		dlg.m_sLabel = CApplication::Get().GetNoteLabel(nId); 
	}

	if (dlg.DoModal() == IDOK)
	{
		if (IsMenuState(nId, CNotesMenuItem::stChecked))
		{
			m_listNotesMenuActions.m_sLabel = dlg.m_sLabel;
			ProcessCheckedMenu(CNotesMenuItem::acLabel);
		}
		else
		{
			CApplication::Get().SetNoteLabel(nId, dlg.m_sLabel.c_str());
		}
	}
	return 0;
}

/*WMU_MENUUPDATE*/
LRESULT CTrayWnd::OnWMMenuUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	menuutils::UpdateMenuWindow((HMENU)wParam);
	return 0;
}

/* ID_TNM_NEWLABEL */
void CTrayWnd::OnNewLabel(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	EndMenu();
	if (IS_NOTE_CMD(m_nSelectedNoteCmd))
	{
		PostMessage(WMU_NEW_LABEL, m_nSelectedNoteCmd);
	}
}

/* ID_TNM_CLEARLABEL */
void CTrayWnd::OnClearLabel(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if (IS_NOTE_CMD(m_nSelectedNoteCmd))
	{
		if (IsMenuState(GET_NOTE_ID_FROM_CMD(m_nSelectedNoteCmd), CNotesMenuItem::stChecked))
		{
			m_listNotesMenuActions.m_sLabel = _T("");
			ProcessCheckedMenu(CNotesMenuItem::acLabel);
		}
		else
		{
			CApplication::Get().SetNoteLabel(GET_NOTE_ID_FROM_CMD(m_nSelectedNoteCmd), _T(""));
		}
		EndMenu();
	}
}

/* LABEL_CMD_FIRST, LABEL_CMD_LAST*/
void CTrayWnd::OnLabelSelected(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if (IS_NOTE_CMD(m_nSelectedNoteCmd))
	{
		_tstring sLabel;
		if (nID == LABEL_CMD_FIRST)
		{
			sLabel = _T("");
		}
		else if (IS_LABEL_CMD(nID))
		{
			int pos = GET_LABEL_ID_FROM_CMD(nID);
			int i = 1;
			for (std::list<_tstring>::iterator it = m_listLabels.begin();
				it != m_listLabels.end(); ++it)
			{
				if (pos == i)
				{
					sLabel = it->c_str();
					break;
				}
				++i;
			}
		}

		if (IsMenuState(GET_NOTE_ID_FROM_CMD(m_nSelectedNoteCmd), CNotesMenuItem::stChecked))
		{
			m_listNotesMenuActions.m_sLabel = sLabel;
			ProcessCheckedMenu(CNotesMenuItem::acLabel);
		}
		else
		{
// 			_tstring label = CApplication::Get().GetNoteLabel(GET_NOTE_ID_FROM_CMD(m_nSelectedNoteCmd));
// 			if (label == sLabel)
// 			{
// 				sLabel = _T(""); // если выбрана та же метка - очистить
// 			}
			CApplication::Get().SetNoteLabel(GET_NOTE_ID_FROM_CMD(m_nSelectedNoteCmd), sLabel.c_str());
		}
		EndMenu();
	}
}

/* ID_TNM_DUPLICATE */
void CTrayWnd::OnNoteDuplicate(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if (IS_NOTE_CMD(m_nSelectedNoteCmd))
	{
		CApplication::Get().DuplicateNote(GET_NOTE_ID_FROM_CMD(m_nSelectedNoteCmd));
	}
}

/* ID_TNM_CHECK */
void CTrayWnd::OnNoteCheck(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if (IS_NOTE_CMD(m_nSelectedNoteCmd))
	{
		int nNoteId = GET_NOTE_ID_FROM_CMD(m_nSelectedNoteCmd);
		CNotesMenuActions::iterator it = m_listNotesMenuActions.find(nNoteId);
		if (it != m_listNotesMenuActions.end())
		{
			ATLASSERT(m_selectedMenu.IsMenu());

			CMenuHandle menu(m_selectedMenu/*it->m_hPopupMenu*/);
			menu.CheckMenuItem(m_nSelectedNoteCmd, MF_BYCOMMAND | MF_CHECKED);

			it->SetState(CNotesMenuItem::stChecked, TRUE);

			menuutils::UpdateMenuWindow(menu);

		}
	}
}

/* ID_TNM_CHECKALL */
void CTrayWnd::OnNoteCheckAll(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if (IS_NOTE_CMD(m_nSelectedNoteCmd))
	{
		int nNoteId = GET_NOTE_ID_FROM_CMD(m_nSelectedNoteCmd);
		CNotesMenuActions::iterator it = m_listNotesMenuActions.find(nNoteId);
		if (it != m_listNotesMenuActions.end())
		{
			ATLASSERT(m_selectedMenu.IsMenu());
			CMenuHandle menu(m_selectedMenu/*it->m_hPopupMenu*/);
			for (int i = 0; i < menu.GetMenuItemCount(); ++i)
			{
				int itemCmd = menu.GetMenuItemID(i);
				if (IS_NOTE_CMD(itemCmd))
				{
					CNotesMenuActions::iterator iter = m_listNotesMenuActions.find(GET_NOTE_ID_FROM_CMD(itemCmd));
					if (iter != m_listNotesMenuActions.end())
					{
						menu.CheckMenuItem(itemCmd, MF_BYCOMMAND | MF_CHECKED);
						iter->SetState(CNotesMenuItem::stChecked, TRUE);
					}
				}
			}

			menuutils::UpdateMenuWindow(menu);
		}
	}
}

/* ID_TNM_UNCHECK */
void CTrayWnd::OnNoteUncheck(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if (IS_NOTE_CMD(m_nSelectedNoteCmd))
	{
		int nNoteId = GET_NOTE_ID_FROM_CMD(m_nSelectedNoteCmd);
		CNotesMenuActions::iterator it = m_listNotesMenuActions.find(nNoteId);
		if (it != m_listNotesMenuActions.end())
		{
			ATLASSERT(m_selectedMenu.IsMenu());
			CMenuHandle menu(m_selectedMenu/*it->m_hPopupMenu*/);
			menu.CheckMenuItem(m_nSelectedNoteCmd, MF_BYCOMMAND | MF_UNCHECKED);
			it->SetState(CNotesMenuItem::stChecked, FALSE);

			menuutils::UpdateMenuWindow(menu);
		}
	}
}

/* ID_TNM_UNCHECKALL */
void CTrayWnd::OnNoteUnCheckAll(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if (IS_NOTE_CMD(m_nSelectedNoteCmd))
	{
		int nNoteId = GET_NOTE_ID_FROM_CMD(m_nSelectedNoteCmd);
		CNotesMenuActions::iterator it = m_listNotesMenuActions.find(nNoteId);
		if (it != m_listNotesMenuActions.end())
		{
			ATLASSERT(m_selectedMenu.IsMenu());
			CMenuHandle menu(m_selectedMenu/*it->m_hPopupMenu*/);
			for (int i = 0; i < menu.GetMenuItemCount(); ++i)
			{
				int itemCmd = menu.GetMenuItemID(i);
				if (IS_NOTE_CMD(itemCmd))
				{
					CNotesMenuActions::iterator iter = m_listNotesMenuActions.find(GET_NOTE_ID_FROM_CMD(itemCmd));
					if (iter != m_listNotesMenuActions.end())
					{
						menu.CheckMenuItem(itemCmd, MF_BYCOMMAND | MF_UNCHECKED);
						iter->SetState(CNotesMenuItem::stChecked, FALSE);
					}
				}
			}

			menuutils::UpdateMenuWindow(menu);
		}
	}
}


/* ID_TNM_DELETE */
void CTrayWnd::OnNoteDelete(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if (IS_NOTE_CMD(m_nSelectedNoteCmd))
	{
		if (IsMenuState(GET_NOTE_ID_FROM_CMD(m_nSelectedNoteCmd), CNotesMenuItem::stChecked))
		{
			ProcessCheckedMenu(CNotesMenuItem::acDelete);
		}
		else
		{
			CApplication::Get().DeleteNote(GET_NOTE_ID_FROM_CMD(m_nSelectedNoteCmd));
		}
		EndMenu();
	}
}

/* ID_TNM_RESTORE */
void CTrayWnd::OnNoteRestore(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if (IS_NOTE_CMD(m_nSelectedNoteCmd))
	{
		CApplication::Get().RestoreNote(GET_NOTE_ID_FROM_CMD(m_nSelectedNoteCmd));
		EndMenu();
	}
}

/* ID_POPUP_NEWANDPASTE */
void CTrayWnd::OnNewAndPaste(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	TCHAR buf[65536];
	CClipboard::GetText(buf, 65536);
	CApplication::Get().CreateNote(buf, NF_NOACTIVATE | NF_ROLLUP);
}

/**/
bool compare_by_modify_date(CNote const& left, CNote const& right)
{
	time_t leftModifDate = left.GetModifiedDate();
	time_t rigthModifDate = right.GetModifiedDate();
	if (leftModifDate == rigthModifDate)
	{
		return left.GetCreatedDate() < right.GetCreatedDate();
	}
	return leftModifDate < rigthModifDate;
}
/**/
bool compare_by_deleted_date(CNote const& left, CNote const& right)
{
	return left.GetDeletedDate() > right.GetDeletedDate();
}

/* */
BOOL CTrayWnd::SetMenuItemTypeEx(CMenuHandle menu, UINT uItem, BOOL bByPosition, UINT nFlagEx)
{
	CMenuItemInfo mii;
	mii.fMask = MIIM_DATA;
	mii.dwItemData = nFlagEx;
	BOOL res = menu.SetMenuItemInfo(uItem, bByPosition, &mii);
	ATLASSERT(res != FALSE);
	return res;
}

/**/
UINT CTrayWnd::GetMenuItemTypeEx(CMenuHandle menu, UINT uItem, BOOL bByPosition)
{
	UINT res = 0;
	CMenuItemInfo mii;
	mii.fMask = MIIM_DATA;
	menu.GetMenuItemInfo(uItem, bByPosition, &mii);
	res = mii.dwItemData;
#ifdef COOL_CONTEXT_MENU
	res = ((CCoolContextMenu::MenuItemData*)res)->dwItemData;
#endif
	return res;
}

/**/
void CTrayWnd::ModifyNotesMenu(CMenuHandle menuNotes)
{
	int nHiddenNotes = CApplication::Get().GetHiddenNotesCount();
	CNote::List notes;
	CApplication::Get().GetAllNotes(notes, 
		NM_ID | NM_TEXT | NM_MODIFIED | NM_DELETED | NM_LABEL);
	std::sort(notes.begin(), notes.end(), compare_by_modify_date);

	// check notes list
	BOOL bIsNotesExists = FALSE;
	BOOL bIsDeletedNotesExists = FALSE;
	for (int i = 0; i < notes.size(); ++i)
	{
		bIsNotesExists = bIsNotesExists || (notes[i].GetDeletedDate() == 0);
		bIsDeletedNotesExists = bIsDeletedNotesExists || (notes[i].GetDeletedDate() != 0);
		if (bIsNotesExists && bIsDeletedNotesExists)
			break;
	}

	/* check separator */
	int nState = menuNotes.GetMenuState(0, MF_BYPOSITION);
	if (!bIsNotesExists && (nState & MF_SEPARATOR) == MF_SEPARATOR)
	{
		menuNotes.DeleteMenu(0, MF_BYPOSITION);
	}
	if (bIsNotesExists && (nState & MF_SEPARATOR) != MF_SEPARATOR)
	{
		menuNotes.InsertMenu(0, MF_BYPOSITION | MF_SEPARATOR);
	}

	// show all notes
	std::list<_tstring> listLabels;
	CApplication::Get().GetLabels(listLabels);
	bool bLabels = false;
	for (std::list<_tstring>::reverse_iterator it = listLabels.rbegin();
		it != listLabels.rend(); ++it)
	{
		_tstring sLabel = it->c_str();
		CMenuHandle menuLabel;
		menuLabel.CreatePopupMenu();
		for (int i = 0; i < notes.size(); ++i)
		{
			if (notes[i].GetDeletedDate() == 0 && notes[i].GetLabel() == sLabel)
			{
				bLabels = true;
				_tstring sCaption = CApplication::Get().GetNoteCaption(notes[i].GetText());
				int nId = notes[i].GetId();
				int nCmd = CREATE_NOTE_CMD(nId);
				menuLabel.InsertMenu(0, MF_BYPOSITION, nCmd, sCaption.c_str());
				m_listNotesMenuActions.push_back(CNotesMenuItem(nId, menuLabel));
			}
		}
		menuNotes.InsertMenu(0, MF_BYPOSITION | MF_POPUP, menuLabel, sLabel.c_str());
	}
	
	bool bSeparator = false;
	for (int i = 0; i < notes.size(); ++i)
	{
		if (notes[i].GetDeletedDate() == 0 && notes[i].GetLabel().empty())
		{
			if (bSeparator == false && bLabels == true)
			{
				// insert separator
				menuNotes.InsertMenu(0, MF_BYPOSITION | MF_SEPARATOR);
				bSeparator = true;
			}
			_tstring sCaption = CApplication::Get().GetNoteCaption(notes[i].GetText());
			int nId = notes[i].GetId();
			int nCmd = CREATE_NOTE_CMD(nId);
			menuNotes.InsertMenu(0, MF_BYPOSITION, nCmd, sCaption.c_str());
			m_listNotesMenuActions.push_back(CNotesMenuItem(nId, menuNotes));
		}
	}

	// show deleted notes
	CMenuHandle menuDeleted = GetDeletedMenu();
	std::sort(notes.begin(), notes.end(), compare_by_deleted_date);
	if (bIsDeletedNotesExists)
	{
		menuDeleted.DeleteMenu(ID_UNDELETE_EMPTY, MF_BYCOMMAND);
	}
	int nMaxDelCnt = 30;
	for (int i = 0; i < notes.size(); ++i)
	{
		if (notes[i].GetDeletedDate() != 0)
		{
			if (nMaxDelCnt < 0 && 
				(CApplication::Get().IsNoteVisible(notes[i].GetId()) == FALSE) )// note is not opened
			{
				CApplication::Get().DeleteFromStorage(notes[i].GetId());
			}
			else
			{
				_tstring sCaption = CApplication::Get().GetNoteCaption(notes[i].GetText());
				int nId = notes[i].GetId();
				int nCmd = CREATE_NOTE_CMD(nId);
				menuDeleted.AppendMenu(MF_STRING, nCmd, sCaption.c_str());
				SetMenuItemTypeEx(menuDeleted, nCmd, FALSE, MFT_EX_DELETED);
				CNotesMenuItem item(nId, menuDeleted);
				item.SetState(CNotesMenuItem::stDeleted);
				m_listNotesMenuActions.push_back(item);

			}
			--nMaxDelCnt;
		}
	}
}

/* WM_MENUSELECT */
void CTrayWnd::OnMenuSelect(UINT nItemID, UINT nFlags, CMenuHandle menu)
{
	if (!::IsMenu(m_menuNoteActions))
	{
		m_nSelectedNoteCmd = IS_NOTE_CMD(nItemID) ? nItemID : 0;
		if (m_nSelectedNoteCmd)
		{
			m_selectedMenu = menu;
		}
	}

	SetMsgHandled(FALSE);
}

/**/
void CTrayWnd::OnSettings(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if (m_pSettingsDlg.get() != NULL && ::IsWindow(m_pSettingsDlg->m_hWnd))
	{
		::SetForegroundWindow(m_pSettingsDlg->m_hWnd);
	}
	else
	{
		m_pSettingsDlg = std::auto_ptr<CSettingsSheet>(new CSettingsSheet(IDS_SETTINGS));
		m_pSettingsDlg->DoModal(m_hWnd);
		m_pSettingsDlg.reset();
	}
}

/**/
void CTrayWnd::AssociateImage(CMenuItemInfo& mii, MenuItemData * pMI)
{
	if (IS_NOTE_CMD(mii.wID))
	{
		if (mii.dwItemData & MFT_EX_DELETED)
		{
			pMI->iImage = 1;
		}
		else
		{
			pMI->iImage = 0;
		}
	}
	else	
	{
		switch (mii.wID)
		{
		case ID_TNM_DELETE:
			pMI->iImage = 1;
			break;
		case ID_POPUP_NEWNOTE:
			pMI->iImage = 2;
			break;
		case ID_TNM_OPEN_NOTE:
			pMI->iImage = 3;
			break;
		case ID_TNM_COPYALLTOCLIPBOARD:
			pMI->iImage = 4;
			break;
		case ID_TNM_DUPLICATE:
			pMI->iImage = 5;
			break;
		case ID_SETTINGS:
			pMI->iImage = 6;
			break;
		case ID_MORE_HELP:
			pMI->iImage = 7;
			break;
		default:
			pMI->iImage = -1;
			break;
		}
	}
}
