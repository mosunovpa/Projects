
#pragma once

#include "TrayIcon.h"
#include "resource.h"
#include "NotesMenuActions.h"
#include "SettingsSheet.h"

#define NOTE_CMD_FIRST 50000
#define NOTE_CMD_RANGE 10000
#define NOTE_CMD_LAST (NOTE_CMD_FIRST + NOTE_CMD_RANGE)
#define CREATE_NOTE_CMD(id) ((id) + NOTE_CMD_FIRST)
#define GET_NOTE_ID_FROM_CMD(cmd) ((cmd) - NOTE_CMD_FIRST)
#define IS_NOTE_CMD(cmd) (cmd > NOTE_CMD_FIRST && cmd <= NOTE_CMD_LAST)

/*
#define DELETED_CMD_FIRST NOTE_CMD_LAST
#define DELETED_CMD_LAST (DELETED_CMD_FIRST + NOTE_CMD_RANGE)
#define CREATE_DELETED_CMD(id) ((id) + DELETED_CMD_FIRST)
#define GET_DELETED_ID_FROM_CMD(cmd) ((cmd) - DELETED_CMD_FIRST)
*/

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
	CBitmap m_bmpDeleted;

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
		MSG_WM_MENUSELECT(OnMenuSelect)
		MESSAGE_HANDLER_EX(WMU_NOTIFYICON, OnNotifyIcon)
		COMMAND_ID_HANDLER_EX(ID_POPUP_NEWNOTE, OnPopupNewnote)
		COMMAND_ID_HANDLER_EX(ID_POPUP_NEWANDPASTE, OnNewAndPaste)
		COMMAND_ID_HANDLER_EX(ID_POPUP_SHOWALLNOTES, OnPopupShowAllnotes)
		COMMAND_ID_HANDLER_EX(ID_POPUP_ABOUT, OnPopupAbout)
		COMMAND_ID_HANDLER_EX(ID_POPUP_EXIT, OnPopupExit)
		COMMAND_ID_HANDLER_EX(ID_SETTINGS, OnSettings)
		COMMAND_ID_HANDLER_EX(ID_TNM_OPEN_NOTE, OnOpenNote);
		COMMAND_ID_HANDLER_EX(ID_TNM_COPYALLTOCLIPBOARD, OnCopyAllToClipboard);
		COMMAND_ID_HANDLER_EX(ID_TNM_CHECK, OnNoteCheck);
		COMMAND_ID_HANDLER_EX(ID_TNM_UNCHECK, OnNoteUncheck);
		COMMAND_ID_HANDLER_EX(ID_TNM_DELETE, OnNoteDelete);
		COMMAND_ID_HANDLER_EX(ID_TNM_RESTORE, OnNoteRestore);
		COMMAND_ID_HANDLER_EX(ID_TNM_CHECKED_OPEN, OnCheckedOpen);
		COMMAND_ID_HANDLER_EX(ID_TNM_CHECKED_DELETE, OnCheckedDelete);
		COMMAND_RANGE_HANDLER_EX(NOTE_CMD_FIRST + 1, NOTE_CMD_LAST, OnNoteSelected)
	}
	CATCH_ALL_ERRORS(m_hWnd)
	END_MSG_MAP_EX()

	LRESULT OnCreate(LPCREATESTRUCT lpCreateStruct);
	LRESULT OnDestroy(void);
	LRESULT	OnQueryEndSession(UINT wParam, UINT lParam);
	void OnFocus(HWND hWnd);
	void OnMenuRButtonUp(WPARAM wParam, CMenuHandle menu);
	void OnMenuSelect(UINT nItemID, UINT nFlags, CMenuHandle menu);
	LRESULT OnNotifyIcon(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnPopupNewnote(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnNewAndPaste(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnPopupShowAllnotes(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnPopupAbout(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnPopupExit(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnSysCommand(UINT nID, CPoint pt);
	void OnNoteSelected(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnOpenNote(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnCopyAllToClipboard(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnNoteCheck(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnNoteUncheck(UINT uNotifyCode, int nID, CWindow wndCtl);

	void OnNoteDelete(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnNoteRestore(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnCheckedOpen(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnCheckedDelete(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnSettings(UINT uNotifyCode, int nID, CWindow wndCtl);

private:
	LRESULT DisplayShortcutMenu();
	void CreateBitmaps();
	void ModifyNotesMenu(CMenuHandle menuNotes);
	void SetNotesMenuActions(CNotesMenuItem::Actions action);
	void ProcessNotesMenuActions();

	int m_nSelectedMenuItemId;
	CMenu m_menuNoteActions;

	CToolTipCtrl m_tooltip;
	CNotesMenuActions m_listNotesMenuActions;

	std::auto_ptr<CSettingsSheet> m_pSettingsDlg;


};