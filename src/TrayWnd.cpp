#include "stdafx.h"
#include "TrayWnd.h"
#include "resutils.h"
#include "Application.h"

CTrayWnd::CTrayWnd()
{

}

CTrayWnd::~CTrayWnd()
{
	// Destroy the menu
	if (m_menuPopup.m_hMenu)
		m_menuPopup.DestroyMenu();
}

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
	return 0;
}

LRESULT CTrayWnd::OnDestroy(void)
{
	// Delete the icon from the taskbar status area
	if (!m_TrayIcon.RemoveIconFromTaskBar(m_hWnd, IDR_MAINFRAME))
		ATLTRACE(_T("Icon was not removed from the taskbar!\n"));

	::PostQuitMessage(0);
	SetMsgHandled(FALSE);

	return 0;
}

LRESULT CTrayWnd::OnQueryEndSession(UINT wParam, UINT lParam)
{
	OnDestroy();
	return TRUE;
}

LRESULT CTrayWnd::OnNotifyIcon(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// Determine the tray message
	switch (lParam)
	{
		// Right mouse button click - display a popup menu
	case WM_RBUTTONUP:
		DisplayShortcutMenu();
		break;
		// Left mouse button double-click - create a new note
	case WM_LBUTTONDBLCLK:
		CApplication::Get().CreateNote();
		break;
		// activate opened notes
	case WM_LBUTTONUP:
		SetForegroundWindow(m_hWnd);
		break;
	default:
		break;
	}
	return 0;
}


LRESULT CTrayWnd::DisplayShortcutMenu()
{
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
	MENUITEMINFO mif;
	ZeroMemory(&mif, sizeof(MENUITEMINFO));
	mif.cbSize = sizeof(MENUITEMINFO);
	mif.fMask = MIIM_STATE;
	mif.fState = MFS_DEFAULT;
	::SetMenuItemInfo(menuTrackPopup, ID_POPUP_NEWNOTE, FALSE, &mif);

	if (CApplication::Get().GetOpenedNotesCount() == 0)
	{
		menuTrackPopup.DeleteMenu(ID_POPUP_HIDEALLNOTES, MF_BYCOMMAND);
	}
	if (CApplication::Get().IsAllNotesOpened())
	{
		menuTrackPopup.DeleteMenu(ID_POPUP_SHOWALLNOTES, MF_BYCOMMAND);
	}

	// Display the shortcut menu. Track the right mouse button
	if (!menuTrackPopup.TrackPopupMenu(TPM_RIGHTALIGN|TPM_RIGHTBUTTON, pt.x, pt.y, m_hWnd, NULL))
	{
		ATLTRACE(_T("Shortcut menu was not displayed!\n"));
		return 0;
	}

	::PostMessage(m_hWnd, WM_NULL, 0, 0);

	// Destroy the menu and free any memory that the menu occupies
	menuTrackPopup.DestroyMenu();
	m_menuPopup.DestroyMenu();
	m_menuPopup.m_hMenu = NULL;

	return 0;
}

LRESULT CTrayWnd::OnPopupNewnote(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	CApplication::Get().CreateNote();
	return 0;
}

LRESULT CTrayWnd::OnPopupShowAllnotes(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	CApplication::Get().ShowAllNotes();
	return 0;
}

LRESULT CTrayWnd::OnPopupHideAllnotes(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	CApplication::Get().CloseAllNotes();
	return 0;
}

LRESULT CTrayWnd::OnPopupAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	return 0;
}

LRESULT CTrayWnd::OnPopupExit(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	PostMessage(WM_CLOSE);
	return 0;
}

/*
WM_SETFOCUS
*/
void CTrayWnd::OnFocus(HWND hWnd)
{
	CApplication::Get().RestoreFocus();
	SetMsgHandled(FALSE);
}
