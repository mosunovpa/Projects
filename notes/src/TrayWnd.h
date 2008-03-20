
#pragma once

#include "TrayIcon.h"
#include "resource.h"

#define APP_NAME _T("My Notes")

class CTrayWnd : public CWindowImpl<CTrayWnd> 
{
public:
	CTrayWnd();
	~CTrayWnd();

private:
	CTrayIcon m_TrayIcon;
	CIcon m_Icon;
	CIcon m_IconSm;
	HICON m_hIconNotes;
	CMenu m_menuPopup;

public:

	BEGIN_MSG_MAP_EX(CTrayWnd)
	try
	{
		MSG_WM_CREATE(OnCreate)
		MSG_WM_DESTROY(OnDestroy)
		MSG_WM_QUERYENDSESSION(OnQueryEndSession)
		MSG_WM_SETFOCUS(OnFocus)
		MESSAGE_HANDLER(WMU_NOTIFYICON, OnNotifyIcon)
		COMMAND_ID_HANDLER_EX(ID_POPUP_NEWNOTE, OnPopupNewnote)
		COMMAND_ID_HANDLER_EX(ID_POPUP_SHOWALLNOTES, OnPopupShowAllnotes)
		COMMAND_ID_HANDLER_EX(ID_POPUP_HIDEALLNOTES, OnPopupHideAllnotes)
		COMMAND_ID_HANDLER_EX(ID_POPUP_ABOUT, OnPopupAbout)
		COMMAND_ID_HANDLER_EX(ID_POPUP_EXIT, OnPopupExit)
	}
	CATCH_ALL_ERRORS(m_hWnd)
	END_MSG_MAP_EX()

	LRESULT OnCreate(LPCREATESTRUCT lpCreateStruct);
	LRESULT OnDestroy(void);
	LRESULT OnNotifyIcon(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnQueryEndSession(UINT wParam, UINT lParam);
	LRESULT OnPopupNewnote(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnPopupShowAllnotes(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnPopupHideAllnotes(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnPopupAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnPopupExit(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	void OnFocus(HWND hWnd);
private:
	LRESULT DisplayShortcutMenu();

};