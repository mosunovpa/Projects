
#pragma once

#include "TrayIcon.h"
#include "resource.h"

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
		MSG_WM_SYSCOMMAND(OnSysCommand)
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
	LRESULT OnQueryEndSession(UINT wParam, UINT lParam);
	void OnFocus(HWND hWnd);
	LRESULT OnNotifyIcon(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnPopupNewnote(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnPopupShowAllnotes(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnPopupHideAllnotes(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnPopupAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnPopupExit(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	void OnSysCommand(UINT nID, CPoint pt);
private:
	LRESULT DisplayShortcutMenu(BOOL bRightButton = TRUE);

};