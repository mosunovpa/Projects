/**
 * \file NoteWnd.h
 *
 * \since 2007/01/09
 * \author pavel
 */

#pragma once

#include "StaticSysIcon.h"
#include "resource.h"
#include "menuutils.h"
#include "defines.h"
#include "CoolContextMenu.h"

//////////////////////////////////////////////////////////////////////////
//
class CNoteEdit : public CWindowImpl<CNoteEdit, CRichEditCtrl>,
                    public CRichEditCommands<CNoteEdit>
{
public:
	DECLARE_WND_SUPERCLASS(NULL, CRichEditCtrl::GetWndClassName())
    BEGIN_MSG_MAP_EX(CNoteEdit)
		MESSAGE_HANDLER_EX(WM_KEYDOWN, OnKeyDown)
        CHAIN_MSG_MAP_ALT(CRichEditCommands<CNoteEdit>, 1)
    END_MSG_MAP_EX()

	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (wParam == VK_ESCAPE)
		{
			GetParent().PostMessage(WM_CLOSE);
		}
		if (wParam == 68 && GetKeyState(VK_CONTROL) & 0x8000) // Ctrl+D
		{
			GetParent().PostMessage(WM_COMMAND, ID_REMOVE);
		}
		SetMsgHandled(FALSE);
		return 0;
	}

	class CRichEditOleCallback : public IRichEditOleCallback
	{
	public:
		STDMETHOD_(ULONG, AddRef)() 
		{ 
			return 1; 
		}
		STDMETHOD_(ULONG, Release)() 
		{ 
			return 1; 
		}
		STDMETHOD(QueryInterface)(REFIID riid, LPVOID* ppvObj)
		{
			if (riid == IID_IUnknown || riid == IID_IRichEditOleCallback)  
			{  
				*ppvObj = (void*)this;  
				return S_OK;  
			}  
			else  
			{  
				*ppvObj   =   NULL;  
				return   E_NOINTERFACE;  
			} 
		}
		STDMETHOD(GetNewStorage)(LPSTORAGE* lplpstg)
		{
			return E_NOTIMPL;  
		}
		STDMETHOD(GetInPlaceContext)(LPOLEINPLACEFRAME*, LPOLEINPLACEUIWINDOW*, LPOLEINPLACEFRAMEINFO)
		{
			return E_NOTIMPL;  
		}
		STDMETHOD(ShowContainerUI)(BOOL)
		{
			return E_NOTIMPL;  
		}
		STDMETHOD(QueryInsertObject)(LPCLSID, LPSTORAGE, LONG)
		{
			return E_NOTIMPL;  
		}
		STDMETHOD(DeleteObject)(LPOLEOBJECT)
		{
			return E_NOTIMPL;  
		}
		STDMETHOD(QueryAcceptData)(LPDATAOBJECT, CLIPFORMAT* pcfFormat, DWORD,BOOL, HGLOBAL)
		{
			*pcfFormat = CF_TEXT;
			return S_OK;  
		}
		STDMETHOD(ContextSensitiveHelp)(BOOL)
		{
			return E_NOTIMPL;  
		}
		STDMETHOD(GetClipboardData)(CHARRANGE*, DWORD, LPDATAOBJECT*)
		{
			return E_NOTIMPL;  
		}
		STDMETHOD(GetDragDropEffect)(BOOL, DWORD, LPDWORD)
		{
			return E_NOTIMPL;  
		}
		STDMETHOD(GetContextMenu)(WORD, LPOLEOBJECT, CHARRANGE*, HMENU* hMenu)
		{
			CMenuHandle hEditMenu;
			hEditMenu.LoadMenu(IDR_EDIT);
			CMenuHandle menuPopup = hEditMenu.GetSubMenu(0);
			*hMenu = menuPopup.m_hMenu;
			return S_OK;  
		}
	} m_OleCallback;
};

//////////////////////////////////////////////////////////////////////////
//

class CNoteWnd : public CWindowImpl<CNoteWnd>,
	public CCoolContextMenu<CNoteWnd>
{
public:
	CNoteWnd(int nNoteId = 0);
	virtual ~CNoteWnd();

	int GetId() const;
	void SetId(int id);
	_tstring GetText() const;
	void SetText(_tstring const& text);
	time_t GetCreatedDate() const;
	void SetCreatedDate(time_t dt);
	time_t GetDeletedDate() const;
	void SetDeletedDate(time_t dt);
	_tstring GetLabel() const;
	void SetLabel(LPCTSTR label);
	void SetInitFlags(DWORD nFlags);

	CNoteEdit& GetEditor();
	CRect GetRealNoteRect();
	void OptionsUpdated();
	BOOL IsMinimized();
	void Rollup();
	void Unroll();
	void ShowSystemMenu(CPoint pt);
	void ShowLabelMenu(CPoint pt);
	void EscapeFocus();
	void Refresh();
	void SetReadOnly(BOOL bReadOnly = TRUE);

	DECLARE_WND_CLASS(NOTE_WND_CLASS_NAME)

	BEGIN_MSG_MAP_EX(CNoteWnd)
		try
		{
			MSG_WM_CREATE(OnCreate)
			MSG_WM_DESTROY(OnDestroy)
			MESSAGE_HANDLER_EX(WMU_INITNOTE, OnInitNote)
			MSG_WM_NCHITTEST(OnNcHittest)
 			MSG_WM_NCPAINT(OnNcPaint)
			MSG_WM_PAINT(OnPaint)
			MSG_WM_NCACTIVATE(OnNcActivate)
			MSG_WM_ACTIVATE(OnActivate)
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
			MSG_WM_MENURBUTTONUP(OnMenuRButtonUp)

			MSG_WM_MOUSEACTIVATE(OnMouseActivate)
			MSG_WM_LBUTTONUP(OnLButtonUp)
			MSG_WM_ENTERSIZEMOVE(OnEnterSizeMove)
			MSG_WM_EXITSIZEMOVE(OnExitSizeMove)
			MSG_WM_NCLBUTTONDOWN(OnNcLButtonDown)

			COMMAND_ID_HANDLER_EX(ID_CLIPBRD_COPY, OnCopyToClipboard)
			COMMAND_ID_HANDLER_EX(ID_RESTORE, OnRestore)
			COMMAND_ID_HANDLER_EX(ID_DUPLICATE, OnDuplicate)
			COMMAND_RANGE_HANDLER_EX(LABEL_CMD_FIRST, LABEL_CMD_LAST, OnLabelSelected)
			COMMAND_ID_HANDLER_EX(ID_LABEL_NEWLABEL, OnNewLabel);
			COMMAND_ID_HANDLER_EX(ID_LABEL_CLEAR, OnLabelClear);
			COMMAND_ID_HANDLER_EX(ID_PASTE, OnPaste)
			COMMAND_ID_HANDLER_EX(ID_CLOSEALL, OnNoteCloseAll)
			COMMAND_ID_HANDLER_EX(ID_CLOSEALLBUTTHIS, OnNoteCloseAllButThis)
			COMMAND_ID_HANDLER_EX(ID_CLOSE, OnNoteClose)
			COMMAND_ID_HANDLER_EX(ID_DELETE, OnNoteDelete)
			COMMAND_ID_HANDLER_EX(ID_REMOVE, OnNoteDelete)
			COMMAND_ID_HANDLER_EX(ID_ROLLUP, OnRollUp)
			COMMAND_ID_HANDLER_EX(ID_UNROLL, OnUnroll)
			NOTIFY_CODE_HANDLER_EX(EN_LINK, OnLink)
			CHAIN_COMMANDS_MEMBER(m_edit)

			CHAIN_MSG_MAP(CCoolContextMenu<CNoteWnd>)
		} 
		CATCH_ALL_ERRORS(m_hWnd)
	END_MSG_MAP_EX()


	virtual void OnFinalMessage(HWND hWnd);

	LRESULT OnCreate(LPCREATESTRUCT lParam);
	void OnDestroy();
	LRESULT OnInitNote(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnNcHittest(CPoint pt);
	void OnPaint(HDC hdc);
	BOOL OnNcActivate(BOOL bActive);
	void OnActivate(UINT nState, BOOL bMinimized, HWND hWndOther);
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
	void OnMenuRButtonUp(WPARAM wParam, CMenuHandle menu);

	int OnMouseActivate(CWindow wndTopLevel, UINT nHitTest, UINT message);
	void OnLButtonUp(UINT nFlags, CPoint point);
	void OnEnterSizeMove();
	void OnExitSizeMove();
	void OnNcLButtonDown(UINT nHitTest, CPoint point);

	void OnCopyToClipboard(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnRestore(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnDuplicate(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnLabelSelected(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnNewLabel(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnLabelClear(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnPaste(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnNoteCloseAll(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnNoteCloseAllButThis(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnNoteClose(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnNoteDelete(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnRollUp(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnUnroll(UINT uNotifyCode, int nID, CWindow wndCtl);
	LRESULT OnLink(LPNMHDR pnmh);
	void AssociateImage(CMenuItemInfo& mii, MenuItemData * pMI);

private:
	void PopulateLabelMenu(CMenuHandle menuLabels);

	CRect GetIconRect();
	CRect GetCaptionRect();
	int GetMinimizedHeight();
	CRect GetBottomRightRect();
	CRect GetRollupButtonRect();
	CRect GetCloseButtonRect();
	CRect GetClientRect();
	CMenuHandle AdjustSystemMenu();
	void StoreNote();
	void DrawStatusBar(CDC& dc);
	void DrawTextInCaption(CDC& dc, const _tstring& text, COLORREF color);

	static CBrush m_hBgBrush;
	static CIcon m_hIcon;
	static CIcon m_hIconSm;
	static CIcon m_hIconTrash;
	static CPen m_hPen;
	static CPen m_hGrayPen;
	static CFont m_hStatusFont;

	CNoteEdit m_edit;
	CEdit m_editCreated;
	CStaticSysIcon m_icon;
	CBitmapButton m_btnRollUp;
	CBitmapButton m_btnUnroll;
	CBitmapButton m_btnClose;
		
	int m_nNoteId;
	
	DWORD m_flagSave;

	time_t m_dtCreated;
	time_t m_dtDeleted;
	_tstring m_label;

	BOOL m_bMinimized;
	CRect m_rcRestored;
	BOOL m_bInitialized;

	BOOL m_bActive;
	std::list<_tstring> m_listLabels;
	DWORD m_flagInit;

	BOOL m_bPrevActive;
	CMenuHandle m_activeMenu;
};