
#pragma once

#include "TrayIcon.h"
#include "resource.h"
#include "user_messages.h"
#include "SettingsSheet.h"
#include "MenuTooltip.h"
#include "defines.h"
#include "CoolContextMenu.h"
#include "NotesView.h"

/* CTrayWnd */
class CTrayWnd : 
	public CWindowImpl<CTrayWnd>,
	public CMenuTooltip<CTrayWnd>,
	public CNotesView<CTrayWnd>
#ifdef COOL_CONTEXT_MENU
	,
	public CCoolContextMenu<CTrayWnd>
#endif
{
public:
	CTrayWnd();
	~CTrayWnd();
	HMENU GetTooltipMenu() const;
	_tstring GetTooltipText(int nSelectedMenuItemId) const;
	BOOL BeforeTooltipShowing() const;

private:
	CTrayIcon m_TrayIcon;
	CIcon m_Icon;
	CIcon m_IconSm;
	HICON m_hIconNotes;
	CMenu m_menuPopup;
	CBitmap m_bmpDeleted;

public:
	DECLARE_WND_CLASS(_T("TrayWnd"))

	BEGIN_MSG_MAP(CTrayWnd)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_DESTROY(OnDestroy)
		MSG_WM_QUERYENDSESSION(OnQueryEndSession)
		MSG_WM_SETFOCUS(OnFocus)
		MSG_WM_SYSCOMMAND(OnSysCommand)
		MSG_WM_MENURBUTTONUP(OnMenuRButtonUp)
		MSG_WM_MENUSELECT(OnMenuSelect)
		MSG_WM_INITMENUPOPUP(OnInitMenuPopup)
		MSG_WM_UNINITMENUPOPUP(OnUnInitMenuPopup)
		MESSAGE_HANDLER_EX(WMU_NOTIFYICON, OnNotifyIcon)
		//MESSAGE_HANDLER_EX(WMU_NEW_LABEL, OnWMUNewLabel)
		MESSAGE_HANDLER_EX(WMU_DISPLAY_SHORCUT_MENU, OnDisplayShortcutMenu)
		COMMAND_ID_HANDLER_EX(ID_POPUP_NEWNOTE, OnPopupNewnote)
		COMMAND_ID_HANDLER_EX(ID_POPUP_NEWANDPASTE, OnNewAndPaste)
		COMMAND_ID_HANDLER_EX(ID_POPUP_ABOUT, OnPopupAbout)
		COMMAND_ID_HANDLER_EX(ID_POPUP_EXIT, OnPopupExit)
		COMMAND_ID_HANDLER_EX(ID_OPENNOTES, OnOpenNotes)
		COMMAND_ID_HANDLER_EX(ID_SETTINGS, OnSettings)
		COMMAND_ID_HANDLER_EX(ID_TNM_OPEN_NOTE, OnOpenNote);
		COMMAND_ID_HANDLER_EX(ID_TNM_COPYALLTOCLIPBOARD, OnCopyAllToClipboard);
		COMMAND_ID_HANDLER_EX(ID_TNM_NEWLABEL, OnLabelSelected/*OnNewLabel*/)
		COMMAND_RANGE_HANDLER_EX(LABEL_CMD_FIRST, LABEL_CMD_LAST, OnLabelSelected)
		//COMMAND_ID_HANDLER_EX(ID_TNM_CLEARLABEL, OnClearLabel)
		COMMAND_ID_HANDLER_EX(ID_TNM_NOTEBOOK, OnMoveToNotebook)
		COMMAND_RANGE_HANDLER_EX(MOVE_TO_NOTEBOOK_CMD_FIRST, MOVE_TO_NOTEBOOK_CMD_LAST, OnMoveToNotebook);
		COMMAND_ID_HANDLER_EX(ID_TNM_DUPLICATE, OnNoteDuplicate);
		COMMAND_ID_HANDLER_EX(ID_TNM_CHECK, OnNoteCheck);
		COMMAND_ID_HANDLER_EX(ID_TNM_CHECKALL, OnNoteCheckAll);
		COMMAND_ID_HANDLER_EX(ID_TNM_UNCHECK, OnNoteUncheck);
		COMMAND_ID_HANDLER_EX(ID_TNM_UNCHECKALL, OnNoteUnCheckAll);
		COMMAND_ID_HANDLER_EX(ID_TNM_DELETE, OnNoteDelete);
		COMMAND_ID_HANDLER_EX(ID_TNM_REMOVE, OnNoteDelete);
		COMMAND_ID_HANDLER_EX(ID_TNM_RESTORE, OnNoteRestore);
		COMMAND_ID_HANDLER_EX(ID_NOTEBOOK_OPEN, OnNotebookOpen);
		COMMAND_RANGE_HANDLER_EX(NOTE_CMD_FIRST + 1, NOTE_CMD_LAST, OnNoteSelected);
		COMMAND_RANGE_HANDLER_EX(NOTEBOOK_CMD_FIRST, NOTEBOOK_CMD_LAST, OnNotebookOpen);

		CHAIN_MSG_MAP(CMenuTooltip<CTrayWnd>)
#ifdef COOL_CONTEXT_MENU
		CHAIN_MSG_MAP(CCoolContextMenu<CTrayWnd>)
#endif
	END_MSG_MAP()

	LRESULT OnCreate(LPCREATESTRUCT lpCreateStruct);
	LRESULT OnDestroy(void);
	LRESULT	OnQueryEndSession(UINT wParam, UINT lParam);
	void OnFocus(HWND hWnd);
	void OnMenuRButtonUp(WPARAM wParam, CMenuHandle menu);
	void OnMenuSelect(UINT nItemID, UINT nFlags, CMenuHandle menu);
	void OnInitMenuPopup(CMenuHandle menuPopup, UINT nIndex, BOOL bSysMenu);
	void OnUnInitMenuPopup(UINT nID, CMenuHandle menu);
	LRESULT OnNotifyIcon(UINT uMsg, WPARAM wParam, LPARAM lParam);
	//LRESULT OnWMUNewLabel(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnDisplayShortcutMenu(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnPopupNewnote(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnNewAndPaste(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnPopupAbout(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnPopupExit(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnSysCommand(UINT nID, CPoint pt);
	void OnNoteSelected(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnOpenNote(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnCopyAllToClipboard(UINT uNotifyCode, int nID, CWindow wndCtl);
	//void OnNewLabel(UINT uNotifyCode, int nID, CWindow wndCtl);
	//void OnClearLabel(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnLabelSelected(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnMoveToNotebook(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnNoteDuplicate(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnNoteCheck(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnNoteCheckAll(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnNoteUncheck(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnNoteUnCheckAll(UINT uNotifyCode, int nID, CWindow wndCtl);

	void OnNoteDelete(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnNoteRestore(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnNotebookOpen(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnSettings(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnOpenNotes(UINT uNotifyCode, int nID, CWindow wndCtl);

	void AssociateImage(CMenuItemInfo& mii, MenuItemData * pMI);
	void GetSelectedNotes(std::list<int>& notes);

private:

	LRESULT DisplayShortcutMenu();
	void ModifyNotesMenu(CMenuHandle menuNotes);
	void PopulateMoveToNotebooksMenu(CMenuHandle menuNotebooks);
	BOOL SetMenuItemTypeEx(CMenuHandle menu, UINT uItem, BOOL bByPosition, UINT nFlagEx);
	UINT GetMenuItemTypeEx(CMenuHandle menu, UINT uItem, BOOL bByPosition);
	void UncheckAll(CMenuHandle menu);

	CMenuHandle GetMenuNotes() const;
	CMenuHandle GetSubMenu(int offset) const;

	int m_nSelectedNoteCmd;
	CMenuHandle m_menuWithChecked;
	CMenuHandle m_selectedMenu;

	CMenu m_menuNoteActions;

	CPoint m_menupoint;

};