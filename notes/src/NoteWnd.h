/**
 * \file NoteWnd.h
 *
 * \since 2007/01/09
 * \author pavel
 */

#pragma once

//#include "StaticSysIcon.h"
#include "resource.h"
#include "user_messages.h"
#include "menuutils.h"
#include "defines.h"
#include "CoolContextMenu.h"
#include "CaptionButton.h"
#include "NotesView.h"

#if defined(SCINTILLA)
	#include "ScintillaNoteEdit.h"
	typedef CScintillaNoteEdit CNoteEdit;
#else 
	#include "RichNoteEdit.h"
	typedef CRichNoteEdit CNoteEdit;
#endif

	
//////////////////////////////////////////////////////////////////////////
//

class CSysIconImageList : public CImageList
{
public:
	CSysIconImageList() 
	{
		CreateFromImage(IDB_NOTE_SYS_ICON, 16, 1, CLR_DEFAULT, IMAGE_BITMAP, LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	}
	~CSysIconImageList() 
	{
		Destroy();
	}
};

//////////////////////////////////////////////////////////////////////////
//

class CNoteWnd : public CWindowImpl<CNoteWnd>,
	public CCoolContextMenu<CNoteWnd>,
	public CCaptionButton<CNoteWnd>,
	public CNotesView<CNoteWnd>
{
public:
	CNoteWnd(int nNoteId = 0);
	virtual ~CNoteWnd();

	int GetId() const;
	void SetId(int id);
	_tstring GetText();
	void SetText(_tstring const& text);
	_timeb GetCreatedDate() const;
	void SetCreatedDate(_timeb dt);
	_timeb GetDeletedDate() const;
	void SetDeletedDate(_timeb dt);
	_tstring GetLabel() const;
	void SetLabel(LPCTSTR label);
	void SetInitFlags(DWORD nFlags);

//	CNoteEdit& GetEditor();
	CRect GetRealNoteRect();
	void OptionsUpdated();
	BOOL IsMinimized();
	BOOL IsDeleted();
	void Rollup();
	void Unroll();
	void ShowSystemMenu(CPoint pt);
	void ShowLabelMenu(CPoint pt);
	void EscapeFocus();
	void Refresh();
	void SetReadOnly(BOOL bReadOnly = TRUE);

	DECLARE_WND_CLASS_EX(NOTE_WND_CLASS_NAME, CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_DROPSHADOW, COLOR_WINDOW)

	BEGIN_MSG_MAP(CNoteWnd)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_CLOSE(OnClose)
		MSG_WM_DESTROY(OnDestroy)
		MESSAGE_HANDLER_EX(WMU_INITNOTE, OnInitNote)
		MESSAGE_HANDLER_EX(WMU_ESCAPEFOCUS, OnEscapeFocus)
		MESSAGE_HANDLER_EX(WMU_ACTIVATEPOST, OnActivatePost)
		MESSAGE_HANDLER_EX(WMU_LBUTTONDBLCLK, OnSysMenuDblClk);
		MSG_WM_NCHITTEST(OnNcHittest)
		MSG_WM_NCPAINT(OnNcPaint)
		MSG_WM_PAINT(OnPaint)
		MSG_WM_NCACTIVATE(OnNcActivate)
		MSG_WM_ACTIVATE(OnActivate)
		MSG_WM_ACTIVATEAPP(OnActivateApp)
		MSG_WM_ERASEBKGND(OnErasebkgnd)
		MSG_WM_GETMINMAXINFO(OnGetMinMaxInfo)
		MSG_WM_SIZE(OnSize)
		MSG_WM_SETFOCUS(OnFocus)
		MSG_WM_MOVE(OnMove)
		MSG_WM_CTLCOLORSTATIC(OnCtlColorStatic)
		MSG_WM_CTLCOLOREDIT(OnCtlColorEdit)
		MSG_WM_INITMENUPOPUP(OnInitMenuPopup)
		MSG_WM_NCLBUTTONDBLCLK(OnNcLButtonDblClk)
		MSG_WM_CONTEXTMENU(OnContextMenu)
		MSG_WM_NCRBUTTONUP(OnNcRButtonUp)

		MSG_WM_ENTERSIZEMOVE(OnEnterSizeMove)

		MSG_WM_LBUTTONUP(OnLButtonUp)
		MSG_WM_LBUTTONDBLCLK(OnLButtonDblClk)
		MSG_WM_SYSCOMMAND(OnSysCommand)

		// не нужно пока
		//MSG_WM_EXITSIZEMOVE(OnExitSizeMove)
		//MSG_WM_NCLBUTTONDOWN(OnNcLButtonDown)
		//MSG_WM_NCLBUTTONUP(OnNcLButtonUp)
		//MSG_WM_MOUSEACTIVATE(OnMouseActivate)

		COMMAND_ID_HANDLER_EX(ID_CLIPBRD_COPY, OnCopyToClipboard)
		COMMAND_ID_HANDLER_EX(ID_RESTORE, OnRestore)
		COMMAND_ID_HANDLER_EX(ID_DUPLICATE, OnDuplicate)
		COMMAND_ID_HANDLER_EX(ID_LABEL_NEWLABEL, OnLabelSelected/*OnNewLabel*/)
		COMMAND_RANGE_HANDLER_EX(LABEL_CMD_FIRST, LABEL_CMD_LAST, OnLabelSelected)
		//COMMAND_ID_HANDLER_EX(ID_LABEL_CLEAR, OnLabelClear)
		COMMAND_ID_HANDLER_EX(ID_PASTE, OnPaste)
		COMMAND_ID_HANDLER_EX(ID_CLOSEALL, OnNoteCloseAll)
		COMMAND_ID_HANDLER_EX(ID_CLOSEALLBUTTHIS, OnNoteCloseAllButThis)
		COMMAND_ID_HANDLER_EX(ID_CLOSE, OnNoteClose)
		COMMAND_ID_HANDLER_EX(ID_DELETE, OnNoteDelete)
		COMMAND_ID_HANDLER_EX(ID_REMOVE, OnNoteDelete)
		COMMAND_ID_HANDLER_EX(ID_TNM_NOTEBOOK, OnMoveToNotebook)
		COMMAND_RANGE_HANDLER_EX(MOVE_TO_NOTEBOOK_CMD_FIRST, MOVE_TO_NOTEBOOK_CMD_LAST, OnMoveToNotebook);
		COMMAND_ID_HANDLER_EX(ID_ROLLUP, OnRollUp)
		COMMAND_ID_HANDLER_EX(ID_UNROLL, OnUnroll)
		COMMAND_ID_HANDLER_EX(ID_SYSMENU, OnSysMenu)
		COMMAND_ID_HANDLER_EX(ID_TRASHSYSMENU, OnSysMenu)
		NOTIFY_CODE_HANDLER_EX(EN_LINK, OnLink)
		CHAIN_COMMANDS_MEMBER(m_edit)

		CHAIN_MSG_MAP(CCoolContextMenu<CNoteWnd>)

		CHAIN_MSG_MAP(CCaptionButton<CNoteWnd>)
		
		REFLECT_NOTIFICATIONS();

		MSG_WM_NCLBUTTONDOWN(OnNcLButtonDownDef)

	END_MSG_MAP()


	virtual void OnFinalMessage(HWND hWnd);

	LRESULT OnCreate(LPCREATESTRUCT lParam);
	void OnClose();
	void OnDestroy();
	LRESULT OnInitNote(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnEscapeFocus(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnActivatePost(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnSysMenuDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnNcHittest(CPoint pt);
	void OnPaint(HDC hdc);
	BOOL OnNcActivate(BOOL bActive);
	void OnActivate(UINT nState, BOOL bMinimized, HWND hWndOther);
	int OnMouseActivate(CWindow wndTopLevel, UINT nHitTest, UINT message);
	void OnActivateApp(BOOL bActive, DWORD dwThreadID);
	void OnNcPaint(HRGN wParam);
	BOOL OnErasebkgnd(HDC hdc) { return TRUE; }
	void OnGetMinMaxInfo(LPMINMAXINFO lParam);
	void OnSize(UINT wParam, CSize sz);
	void OnFocus(HWND hWnd);
	void OnMove(CPoint pt);
	HBRUSH OnCtlColorStatic(CDCHandle dc, CStatic wndStatic);
	HBRUSH OnCtlColorEdit(CDCHandle dc, CEdit edit);
	void OnInitMenuPopup(CMenuHandle menuPopup, UINT nIndex, BOOL bSysMenu);
	void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	void OnContextMenu(CWindow wnd, CPoint point);
	void OnNcRButtonUp(UINT nHitTest, CPoint point);

	void OnLButtonUp(UINT nFlags, CPoint point);
	void OnLButtonDblClk(UINT nFlags, CPoint point);
	void OnSysCommand(UINT nID, CPoint pt);
	void OnEnterSizeMove();
	void OnExitSizeMove();
	void OnNcLButtonDown(UINT nHitTest, CPoint point);
	void OnNcLButtonDownDef(UINT nHitTest, CPoint point);
	void OnNcLButtonUp(UINT nHitTest, CPoint point);

	void OnCopyToClipboard(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnRestore(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnDuplicate(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnLabelSelected(UINT uNotifyCode, int nID, CWindow wndCtl);
	//void OnNewLabel(UINT uNotifyCode, int nID, CWindow wndCtl);
	//void OnLabelClear(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnPaste(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnNoteCloseAll(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnNoteCloseAllButThis(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnNoteClose(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnNoteDelete(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnMoveToNotebook(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnRollUp(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnUnroll(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnSysMenu(UINT uNotifyCode, int nID, CWindow wndCtl);
	LRESULT OnLink(LPNMHDR pnmh);
	void AssociateImage(CMenuItemInfo& mii, MenuItemData * pMI);
	POINT CNoteWnd::GetButtonPos(int index);
	void GetSelectedNotes(std::list<int>& notes);

private:
	//void PopulateLabelMenu(CMenuHandle menuLabels);

	CRect GetInnerCaptionRect();
	CRect GetOutterCaptionRect();
	CRect GetSysIconRect();
	int GetMinimizedHeight();
	int GetMinimizedWidth();
	CRect GetBottomBorderRect();
	CRect GetBottomRightRect();
	CRect GetClientRect();
	CMenuHandle AdjustSystemMenu();
	void StoreNote();
	void DrawStatusBar(CDC& dc);
	void DrawSysIcon(CDC& dc);
	void DrawTextInCaption(CDC& dc, const _tstring& text, COLORREF color);

	static CSysIconImageList m_ilSysIcon;
	static CBrush m_hBgBrush;
	static CPen m_hPen;
	static CPen m_hGrayPen;
	static CFont m_hStatusFont;

	CNoteEdit m_edit;

	int m_nNoteId;
	
	DWORD m_flagSave;

	_timeb m_dtCreated;
	_timeb m_dtDeleted;
	_tstring m_label;

	BOOL m_bMinimized;
	CRect m_rcRestored;
	BOOL m_bInitialized;

	BOOL m_bActive;
	BOOL m_bActiveApp;
	//std::list<_tstring> m_listLabels;
	DWORD m_flagInit;

	CMenuHandle m_activeMenu;
	BOOL m_bCaptionClick;
};