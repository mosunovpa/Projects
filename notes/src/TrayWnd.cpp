#include "stdafx.h"
#include "TrayWnd.h"
#include "resutils.h"
#include "Application.h"
#include "menuutils.h"
#include "shlwapi.h"
#include "NewLabelDialog.h"

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
		return menuNotes.GetSubMenu(nCount - 9);
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
		return CApplication::Get().FindNote(GET_NOTE_ID_FROM_CMD(nSelectedMenuItemId)).GetText();
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
	if (m_menuPopup.m_hMenu)
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
	if (!menuTrackPopup.TrackPopupMenu(TPM_RIGHTALIGN | TPM_BOTTOMALIGN, pt.x, pt.y, m_hWnd, NULL))
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
	SetMsgHandled(FALSE);
}

/* WM_UNINITMENUPOPUP */
void CTrayWnd::OnUnInitMenuPopup(UINT nID, CMenuHandle menu)
{
	if (menu.m_hMenu == GetMenuNotes().m_hMenu)
	{
		ProcessNotesMenuActions();
		m_listNotesMenuActions.clear();
	}
	SetMsgHandled(FALSE);
}

/**/
void CTrayWnd::SetNotesMenuActions(CNotesMenuItem::Actions action)
{
	for (CNotesMenuActions::iterator it = m_listNotesMenuActions.begin();
		 it != m_listNotesMenuActions.end(); ++it)
	{
		if (it->IsState(CNotesMenuItem::stChecked))
		{
			it->m_action = action;
		}
	}
}

/**/
void CTrayWnd::ProcessNotesMenuActions()
{
	for (CNotesMenuActions::iterator it = m_listNotesMenuActions.begin();
		it != m_listNotesMenuActions.end(); ++it)
	{
		switch (it->m_action)
		{
		case CNotesMenuItem::acOpen:
			CApplication::Get().ShowNote(it->m_nNoteId);
			break;
		case CNotesMenuItem::acDelete:
			CApplication::Get().DeleteNote(it->m_nNoteId);
			break;
		}
	}
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
void CTrayWnd::PopulateLabelMenu(CMenuHandle menuLabels, _tstring const& sLabel)
{
	m_listLabels.clear();
	CApplication::Get().GetLabels(m_listLabels);

	int nSelCmd = LABEL_CMD_FIRST;
	int pos = m_listLabels.size();
	for (std::list<_tstring>::reverse_iterator it = m_listLabels.rbegin();
		it != m_listLabels.rend(); ++it)
	{
		int nCmd = CREATE_LABEL_CMD(pos);
		menuLabels.InsertMenu(0, MF_BYPOSITION, nCmd, it->c_str());
		if (*it == sLabel)
		{
			nSelCmd = nCmd;
		}
		--pos;
	}
	menuLabels.InsertMenu(0, MF_BYPOSITION, LABEL_CMD_FIRST, resutils::resstring(IDS_NONE).c_str());
	menuLabels.CheckMenuRadioItem(LABEL_CMD_FIRST, LABEL_CMD_LAST, nSelCmd, MF_BYCOMMAND);

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
		CNote note = CApplication::Get().FindNote(GET_NOTE_ID_FROM_CMD(m_nSelectedNoteCmd));

		m_menuNoteActions.LoadMenu(IDR_TRAY_NOTE_MENU);
		CMenuHandle submenu;

		if (it->IsState(CNotesMenuItem::stChecked))
		{
			submenu = m_menuNoteActions.GetSubMenu(2);
			PopulateLabelMenu(submenu.GetSubMenu(1), note.GetLabel());
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
			PopulateLabelMenu(submenu.GetSubMenu(2), note.GetLabel());
		}

		submenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RECURSE, pt.x, pt.y, m_hWnd, NULL);
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
			SetNotesMenuActions(CNotesMenuItem::acOpen);
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
		CApplication::Get().NoteTextToClipboard(GET_NOTE_ID_FROM_CMD(m_nSelectedNoteCmd));
		EndMenu();
	}
}

/* ID_TNM_NEWLABEL */
void CTrayWnd::OnNewLabel(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if (IS_NOTE_CMD(m_nSelectedNoteCmd))
	{
		CNote note = CApplication::Get().FindNote(GET_NOTE_ID_FROM_CMD(m_nSelectedNoteCmd));
		CNewLabelDialog	dlg;
		dlg.m_sLabel = note.GetLabel();
		if (dlg.DoModal() == IDOK)
		{
			note.SetLabel(dlg.m_sLabel.c_str());
			CApplication::Get().SaveNote(note, CApplication::NM_LABEL);
			CApplication::Get().NotesUpdated(CApplication::NM_LABEL);
		}
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
		CNote note = CApplication::Get().FindNote(GET_NOTE_ID_FROM_CMD(m_nSelectedNoteCmd));
		note.SetLabel(sLabel);
		CApplication::Get().SaveNote(note, CApplication::NM_LABEL);
		CApplication::Get().NotesUpdated(CApplication::NM_LABEL);
		EndMenu();
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
			CMenuHandle menu(it->m_hPopupMenu);
			menu.CheckMenuItem(m_nSelectedNoteCmd, MF_BYCOMMAND | MF_CHECKED);
			it->SetState(CNotesMenuItem::stChecked, TRUE);

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
			CMenuHandle menu(it->m_hPopupMenu);
			menu.CheckMenuItem(m_nSelectedNoteCmd, MF_BYCOMMAND | MF_UNCHECKED);
			it->SetState(CNotesMenuItem::stChecked, FALSE);

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
			SetNotesMenuActions(CNotesMenuItem::acDelete);
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
		int nNoteId = GET_NOTE_ID_FROM_CMD(m_nSelectedNoteCmd);
		CApplication::Get().RestoreNote(nNoteId);
		EndMenu();
	}
}

/* ID_POPUP_NEWANDPASTE */
void CTrayWnd::OnNewAndPaste(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	HWND wnd = CApplication::Get().CreateNote();
	CApplication::Get().Command(ID_PASTE, wnd);

	// set focus to the top window in z-order
	TCHAR name[512];
	HWND next_wnd = ::GetWindow(wnd, GW_HWNDFIRST);
	::GetClassName(next_wnd, name, 512);
	HWND parent_wnd = ::GetParent(next_wnd);
	while (next_wnd && 
		(!::IsWindowVisible(next_wnd) 
		|| parent_wnd == m_hWnd
		|| lstrcmp(name, _T("Shell_TrayWnd")) == 0))
	{
		next_wnd = ::GetWindow(next_wnd, GW_HWNDNEXT);
		::GetClassName(next_wnd, name, 512);
		parent_wnd = ::GetParent(next_wnd);
	}
	::SetForegroundWindow(next_wnd);
}

/**/
bool compare_by_modify_date(CNote const& left, CNote const& right)
{
	return left.GetModifiedDate() < right.GetModifiedDate();
}
/**/
bool compare_by_deleted_date(CNote const& left, CNote const& right)
{
	return left.GetDeletedDate() > right.GetDeletedDate();
}

/**/
void CTrayWnd::ModifyNotesMenu(CMenuHandle menuNotes)
{
	int nHiddenNotes = CApplication::Get().GetHiddenNotesCount();
	CNote::List notes;
	CApplication::Get().GetAllNotes(notes, 
		CApplication::NM_ID | CApplication::NM_TEXT | CApplication::NM_MODIFIED | CApplication::NM_DELETED | CApplication::NM_LABEL);
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
				_tstring sCaption = CApplication::Get().GetNoteCaption(notes[i].GetText());
				int nId = notes[i].GetId();
				int nCmd = CREATE_NOTE_CMD(nId);
				menuLabel.InsertMenu(0, MF_BYPOSITION, nCmd, sCaption.c_str());
				m_listNotesMenuActions.push_back(CNotesMenuItem(nId, menuLabel));
			}
		}
		menuNotes.InsertMenu(0, MF_BYPOSITION | MF_POPUP, menuLabel, sLabel.c_str());
	}

	for (int i = 0; i < notes.size(); ++i)
	{
		if (notes[i].GetDeletedDate() == 0 && notes[i].GetLabel().empty())
		{
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
				menuDeleted.AppendMenu(MF_BYPOSITION, nCmd, sCaption.c_str());
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

