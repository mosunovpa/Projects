
#pragma once

#include "TrayIcon.h"
#include "resource.h"

const UINT NOTE_CMD_OFFSET = 50000;

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
	DECLARE_WND_CLASS(_T("TrayWnd"))

	BEGIN_MSG_MAP_EX(CTrayWnd)
	try
	{
		MSG_WM_CREATE(OnCreate)
		MSG_WM_DESTROY(OnDestroy)
		MSG_WM_QUERYENDSESSION(OnQueryEndSession)
		MSG_WM_SETFOCUS(OnFocus)
		MSG_WM_SYSCOMMAND(OnSysCommand)
		MSG_WM_MENURBUTTONUP(OnMenuRButtonUp)
		MESSAGE_HANDLER_EX(WMU_NOTIFYICON, OnNotifyIcon)
		COMMAND_ID_HANDLER_EX(ID_POPUP_NEWNOTE, OnPopupNewnote)
		COMMAND_ID_HANDLER_EX(ID_POPUP_SHOWALLNOTES, OnPopupShowAllnotes)
		COMMAND_ID_HANDLER_EX(ID_POPUP_ABOUT, OnPopupAbout)
		COMMAND_ID_HANDLER_EX(ID_POPUP_EXIT, OnPopupExit)
		COMMAND_ID_HANDLER_EX(ID_POPUP_ALWAYS_ON_TOP, OnAlwaysOnTop);
		COMMAND_ID_HANDLER_EX(ID_OPTIONS_FONT, OnOptionsFont);
		COMMAND_ID_HANDLER_EX(ID_TNM_COPYALLTOCLIPBOARD, OnCopyAllToClipboard);
		COMMAND_ID_HANDLER_EX(ID_TNM_DELETE, OnNoteDelete);
		COMMAND_RANGE_HANDLER_EX(NOTE_CMD_OFFSET + 1, NOTE_CMD_OFFSET + 10000, OnCommandRangeHandlerEX)
	}
	CATCH_ALL_ERRORS(m_hWnd)
	END_MSG_MAP_EX()

	LRESULT OnCreate(LPCREATESTRUCT lpCreateStruct);
	LRESULT OnDestroy(void);
	LRESULT	OnQueryEndSession(UINT wParam, UINT lParam);
	void OnFocus(HWND hWnd);
	void OnMenuRButtonUp(WPARAM wParam, CMenuHandle menu);
	LRESULT OnNotifyIcon(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnPopupNewnote(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnPopupShowAllnotes(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnPopupAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnPopupExit(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	void OnSysCommand(UINT nID, CPoint pt);
	void OnCommandRangeHandlerEX(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnAlwaysOnTop(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnCopyAllToClipboard(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnNoteDelete(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnOptionsFont(UINT uNotifyCode, int nID, CWindow wndCtl);
private:
	LRESULT DisplayShortcutMenu();

	int m_nSelectedNoteId;
};