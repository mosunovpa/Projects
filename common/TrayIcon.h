// TrayIcon.h : implementation of the CTrayIcon class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "stdafx.h"
#include "user_messages.h"


// CTrayIcon class is responsible for adding and removing an icon to the taskbar 
// status area.
class CTrayIcon
{
private:
	NOTIFYICONDATA m_nid;

public:
	CTrayIcon()
	{
		ATLTRACE(_T("CTrayIcon::CTrayIcon()\n"));

		memset(&m_nid, 0, sizeof(NOTIFYICONDATA));
		m_nid.cbSize = sizeof(NOTIFYICONDATA);
	}

	// Adds an icon to the taskbar status area.
	// Returns TRUE if successful, or FALSE otherwise.
	// hWnd - handle to the window to receive callback messages.
	// uID - identifier of the icon.
	// hIcon - handle to the icon to add.
	// lpszTip - tooltip text.
	BOOL AddIconToTaskBar(HWND hWnd, UINT uID, HICON hIcon, LPTSTR lpszTip)
	{
		BOOL bRes;

		m_nid.hWnd = hWnd;
		m_nid.uID = uID;
		m_nid.uFlags = NIF_MESSAGE|NIF_ICON|NIF_TIP;
		m_nid.uCallbackMessage = WMU_NOTIFYICON;
		m_nid.hIcon = hIcon;
		if (lpszTip)
			lstrcpyn(m_nid.szTip, lpszTip, sizeof(m_nid.szTip));
		else
			m_nid.szTip[0] = _T('\0');

		bRes = Shell_NotifyIcon(NIM_ADD, &m_nid);

		if (hIcon)
			DestroyIcon(hIcon);

		return bRes;
	}

	// Deletes an icon from the taskbar status area.
	// Returns TRUE if successful, or FALSE otherwise.
	// hWnd - handle to the window that added the icon.
	// uID - identifier of the icon to delete.
	BOOL RemoveIconFromTaskBar(HWND hWnd, UINT uID)
	{
		m_nid.hWnd = hWnd;
		m_nid.uID = uID;

		return Shell_NotifyIcon(NIM_DELETE, &m_nid);
	}
};