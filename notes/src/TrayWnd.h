
#pragma once

#include "TrayIcon.h"
#include "resource.h"

#define NOTE_CMD_FIRST 50000
#define NOTE_CMD_RANGE 10000
#define NOTE_CMD_LAST (NOTE_CMD_FIRST + NOTE_CMD_RANGE)
#define CREATE_NOTE_CMD(id) ((id) + NOTE_CMD_FIRST)
#define GET_NOTE_ID_FROM_CMD(cmd) ((cmd) - NOTE_CMD_FIRST)
#define IS_NOTE_CMD(cmd) (cmd > NOTE_CMD_FIRST && cmd <= NOTE_CMD_LAST)

#define DELETED_CMD_FIRST NOTE_CMD_LAST
#define DELETED_CMD_LAST (DELETED_CMD_FIRST + NOTE_CMD_RANGE)
#define CREATE_DELETED_CMD(id) ((id) + DELETED_CMD_FIRST)
#define GET_DELETED_ID_FROM_CMD(cmd) ((cmd) - DELETED_CMD_FIRST)

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
		COMMAND_ID_HANDLER_EX(ID_POPUP_NEWANDPASTE, OnNewAndPaste)
		COMMAND_ID_HANDLER_EX(ID_POPUP_SHOWALLNOTES, OnPopupShowAllnotes)
		COMMAND_ID_HANDLER_EX(ID_POPUP_ABOUT, OnPopupAbout)
		COMMAND_ID_HANDLER_EX(ID_POPUP_EXIT, OnPopupExit)
		COMMAND_ID_HANDLER_EX(ID_POPUP_ALWAYS_ON_TOP, OnAlwaysOnTop);
		COMMAND_ID_HANDLER_EX(ID_OPTIONS_FONT, OnOptionsFont);
		COMMAND_ID_HANDLER_EX(ID_TNM_COPYALLTOCLIPBOARD, OnCopyAllToClipboard);
		COMMAND_ID_HANDLER_EX(ID_TNM_DELETE, OnNoteDelete);
		COMMAND_RANGE_HANDLER_EX(NOTE_CMD_FIRST + 1, NOTE_CMD_LAST, OnCommandRangeHandlerEX)
	}
	CATCH_ALL_ERRORS(m_hWnd)
	END_MSG_MAP_EX()

	LRESULT OnCreate(LPCREATESTRUCT lpCreateStruct);
	LRESULT OnDestroy(void);
	LRESULT	OnQueryEndSession(UINT wParam, UINT lParam);
	void OnFocus(HWND hWnd);
	void OnMenuRButtonUp(WPARAM wParam, CMenuHandle menu);
	LRESULT OnNotifyIcon(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnPopupNewnote(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnNewAndPaste(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnPopupShowAllnotes(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnPopupAbout(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnPopupExit(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnSysCommand(UINT nID, CPoint pt);
	void OnCommandRangeHandlerEX(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnAlwaysOnTop(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnCopyAllToClipboard(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnNoteDelete(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnOptionsFont(UINT uNotifyCode, int nID, CWindow wndCtl);
private:
	LRESULT DisplayShortcutMenu();

	int m_nSelectedMenuItemId;
};