/**
 * \file HiddenWnd.cpp
 *
 * \since 2006/10/05
 * \author pavel
 */

#include "StdAfx.h"
#include "resource.h"
#include "HiddenWnd.h"
#include "ApplicationImpl.h"
#include "MainFrm.h"

CHiddenWnd::CHiddenWnd(void)
{
}

CHiddenWnd::~CHiddenWnd(void)
{
	// Destroy the menu
	if (m_menuPopup.m_hMenu)
		m_menuPopup.DestroyMenu();
}

LRESULT CHiddenWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// Load the 'Notes' icon from the application's resources
	m_hIconNotes = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME),
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	ATLASSERT(m_hIconNotes);

	// Add the icon to the taskbar status area
	if (!m_TrayIcon.AddIconToTaskBar(m_hWnd, IDR_MAINFRAME, m_hIconNotes, APP_NAME))
	{
		ATLTRACE(_T("Icon was not added to the taskbar!\n"));
		return 0;
	}
#ifdef _DEBUG
	PostMessage(WMU_NOTIFYICON, 0, WM_LBUTTONUP);
#endif
	return 0;
}

LRESULT CHiddenWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (CApplicationImpl::Get().GetMainViewHwnd())
	{
		::SendMessage(CApplicationImpl::Get().GetMainViewHwnd(), WM_APP_EXIT, 0, 0);
	}

	// Delete the icon from the taskbar status area
	if (!m_TrayIcon.RemoveIconFromTaskBar(m_hWnd, IDR_MAINFRAME))
		ATLTRACE(_T("Icon was not removed from the taskbar!\n"));

	::PostQuitMessage(0);
	return 0;
}

LRESULT CHiddenWnd::OnNotifyIcon(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// Determine the tray message
	switch (lParam)
	{
		// Right mouse button click - display a popup menu
	case WM_RBUTTONUP:
		DisplayShortcutMenu();
		break;
		// Left mouse button double-click - create a new note
//	case WM_LBUTTONDBLCLK:
	case WM_LBUTTONUP:
		ShowExplorer();
		break;
	default:
		break;
	}
	return 0;
}

LRESULT CHiddenWnd::OnEndSession(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	OnDestroy(uMsg, wParam, lParam, bHandled);
	return TRUE;
}

LRESULT CHiddenWnd::DisplayShortcutMenu()
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

LRESULT CHiddenWnd::OnAppExit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	PostMessage(WM_CLOSE);
	return 0;
}

LRESULT CHiddenWnd::OnExplore(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	ShowExplorer();
	return 0;
}

/**
 * 
 */
void CHiddenWnd::ShowExplorer()
{
	HWND hMainWnd = CApplicationImpl::Get().GetMainViewHwnd();
	if (hMainWnd)
	{
		if (::IsIconic(hMainWnd))
		{
			::ShowWindow(hMainWnd, SW_RESTORE);
			::SetForegroundWindow(hMainWnd);
		}
		else
		{
			CWindowDC dc(hMainWnd);
			CClientRect rc(hMainWnd);
			if (RectVisible(dc, rc))
			{
				::ShowWindow(CApplicationImpl::Get().GetMainViewHwnd(), SW_MINIMIZE);
			}
			else
			{
				::SetForegroundWindow(CApplicationImpl::Get().GetMainViewHwnd());
			}
		}
	}
	else
	{
		CMainFrame* wndMain = new CMainFrame(m_controller); // see CMainFrame::OnFinalMessage
		wndMain->CreateView(NULL/*this->m_hWnd*/);
	}
}
