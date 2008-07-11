#include "stdafx.h"
#include "TrayWnd.h"
#include "resutils.h"
#include "Application.h"
#include "menuutils.h"


/* constructor*/
CTrayWnd::CTrayWnd() : m_nSelectedMenuItemId(0)
{

}

/* destructor */
CTrayWnd::~CTrayWnd()
{
	// Destroy the menu
	if (m_menuPopup.m_hMenu)
		m_menuPopup.DestroyMenu();
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

	BOOL bAlwaisOnTop = CApplication::Get().GetOptions().GetAlwaysOnTop();
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
void ModifyNotesMenu(CMenuHandle menuNotes)
{
	int nHiddenNotes = CApplication::Get().GetHiddenNotesCount();

	_tstring sMenu;
	//sMenu.resize(menuNotes.GetMenuStringLen(ID_POPUP_SHOWALLNOTES, MF_BYCOMMAND) + 1);
	//menuNotes.GetMenuString(ID_POPUP_SHOWALLNOTES, &sMenu[0], sMenu.size(), MF_BYCOMMAND);
	//sMenu = strutils::format(_T("%s (%d)"), sMenu.c_str(), nHiddenNotes);
	//menuNotes.ModifyMenu(ID_POPUP_SHOWALLNOTES, MF_BYCOMMAND | MF_STRING, 
	//		(UINT_PTR)ID_POPUP_SHOWALLNOTES, sMenu.c_str());
	menuutils::SetMenuItemEnable(menuNotes, ID_POPUP_SHOWALLNOTES, nHiddenNotes > 0);

	CNote::List notes;
	CApplication::Get().GetAllNotes(notes, CApplication::NM_ID | CApplication::NM_TEXT);

	/* check separator */
	int nState = menuNotes.GetMenuState(0, MF_BYPOSITION);
	if (notes.empty() && (nState & MF_SEPARATOR) == MF_SEPARATOR)
	{
		menuNotes.DeleteMenu(0, MF_BYPOSITION);
	}
	if (!notes.empty() && (nState & MF_SEPARATOR) != MF_SEPARATOR)
	{
		menuNotes.InsertMenu(0, MF_BYPOSITION | MF_SEPARATOR);
	}
	
	// show all notes
	for (int i = 0; i < notes.size(); ++i)
	{
		_tstring csNote = notes[i].GetText();
		_tstring sCaption = strutils::trim_string(notes[i].GetText(), 64);
		if (sCaption.size() > 0 && sCaption.size() < csNote.size())
		{
			sCaption += _T("...");
		}
// 		MENUITEMINFO mii;
// 		mii.cbSize = sizeof(MENUITEMINFO);
// 		mii.fMask = MIIM_TYPE | MIIM_ID | MIIM_DATA;
// 		mii.fType = MFT_STRING;
// 		mii.wID = NOTE_CMD_OFFSET + notes[i].GetId();
// 		mii.dwItemData = notes[i].GetId();
// 		mii.dwTypeData = &sCaption[0];
// 		mii.cch = sCaption.size();
// 		menuNotes.InsertMenuItem(0, TRUE, &mii);
		menuNotes.InsertMenu(0, MF_BYPOSITION, CREATE_NOTE_CMD(notes[i].GetId()), sCaption.c_str());
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
	// Get cursor's position
	POINT pt;
	::GetCursorPos(&pt);

	// Display a shortcut menu at the specified location
	// See "PRB: Menus for Notification Icons Don't Work Correctly" in MSDN
	::SetForegroundWindow(m_hWnd);

	// TrackPopupMenu cannot display the menu bar so get
	// a handle to the first shortcut menu
	CMenuHandle menuTrackPopup = m_menuPopup.GetSubMenu(0);
	if (!menuTrackPopup.m_hMenu)
	{
		ATLTRACE(_T("Handle to the first shortcut menu was not retrieved!\n"));
		m_menuPopup.DestroyMenu();
		m_menuPopup.m_hMenu = NULL;
		return 0;
	}

	// set menu item states
	menuTrackPopup.SetMenuDefaultItem(ID_POPUP_NEWNOTE);

	BOOL bAlwaisOnTop = CApplication::Get().GetOptions().GetAlwaysOnTop();
	menuTrackPopup.CheckMenuItem(ID_POPUP_ALWAYS_ON_TOP, MF_BYCOMMAND | (bAlwaisOnTop ? MF_CHECKED : MF_UNCHECKED));

	ModifyNotesMenu(menuTrackPopup);

	// Display the shortcut menu. Track the right mouse button

	if (!menuTrackPopup.TrackPopupMenu(TPM_RIGHTALIGN, pt.x, pt.y, m_hWnd, NULL))
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

/* ID_POPUP_NEWNOTE */
void CTrayWnd::OnPopupNewnote(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	CApplication::Get().CreateNote();
}

/* ID_POPUP_SHOWALLNOTES */
void CTrayWnd::OnPopupShowAllnotes(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	CApplication::Get().ShowAllNotes();
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

/* ID_POPUP_ALWAYS_ON_TOP */
void CTrayWnd::OnAlwaysOnTop( UINT uNotifyCode, int nID, CWindow wndCtl )
{
	BOOL bAlwaisOnTop = CApplication::Get().GetOptions().GetAlwaysOnTop();
	bAlwaisOnTop = !bAlwaisOnTop;
	SetWindowPos(bAlwaisOnTop ? HWND_TOPMOST : HWND_NOTOPMOST, CRect(0,0,0,0), SWP_NOSIZE | SWP_NOMOVE);
	CApplication::Get().GetOptions().SetAlwaysOnTop(bAlwaisOnTop);
	CApplication::Get().SaveOptions();
}

/* ID_OPTIONS_FONT */
void CTrayWnd::OnOptionsFont(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	CFontDialog dlg(NULL, CF_EFFECTS | CF_SCREENFONTS, NULL, m_hWnd);
	dlg.DoModal();
}

/**/
void CTrayWnd::OnCommandRangeHandlerEX(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	CApplication::Get().ShowNote(GET_NOTE_ID_FROM_CMD(nID));
}

/* WM_MENURBUTTONUP */
void CTrayWnd::OnMenuRButtonUp(WPARAM wParam, CMenuHandle menu)
{
	int m_nSelectedMenuItemId = menu.GetMenuItemID(wParam);
	if (IS_NOTE_CMD(m_nSelectedMenuItemId))
	{
		POINT pt;
		::GetCursorPos(&pt);
		CMenu menuNoteContext;
		menuNoteContext.LoadMenu(IDR_TRAY_NOTE_MENU);
		menuNoteContext.GetSubMenu(0).TrackPopupMenu(TPM_LEFTALIGN | TPM_RECURSE, pt.x, pt.y, m_hWnd, NULL);
	}
}

/* ID_TNM_COPYALLTOCLIPBOARD */
void CTrayWnd::OnCopyAllToClipboard(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if (IS_NOTE_CMD(m_nSelectedMenuItemId))
	{
		CApplication::Get().NoteTextToClipboard(GET_NOTE_ID_FROM_CMD(m_nSelectedMenuItemId));
		m_nSelectedMenuItemId = 0;
		EndMenu();
	}
}

/* ID_TNM_DELETE */
void CTrayWnd::OnNoteDelete(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if (IS_NOTE_CMD(m_nSelectedMenuItemId))
	{
		CApplication::Get().Command(ID_DELETE, GET_NOTE_ID_FROM_CMD(m_nSelectedMenuItemId));
		m_nSelectedMenuItemId = 0;
	}
}

/* ID_POPUP_NEWANDPASTE */
void CTrayWnd::OnNewAndPaste(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	CApplication::Get().Command(ID_PASTE, CApplication::Get().CreateNote());
}
