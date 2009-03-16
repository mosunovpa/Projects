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

#define NOTE_WND_CLASS_NAME _T("MyNoteWnd")

const UINT WM_INITNOTE = WM_USER + 100;

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
			GetParent().PostMessage(uMsg, wParam, lParam);
		}
		else
		{
			SetMsgHandled(FALSE);
		}
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

class CNoteWnd : public CWindowImpl<CNoteWnd>
{
public:
	CNoteWnd(int nNoteId = 0);
	virtual ~CNoteWnd();

	int GetId() const;
	void SetId(int id);
	_tstring GetText() const;
	void SetText(LPCTSTR text);
	time_t GetCreatedDate() const;
	void SetCreatedDate(time_t dt);
	time_t GetModifiedDate() const;
	void SetModifiedDate(time_t dt);
	time_t GetDeletedDate() const;
	void SetDeletedDate(time_t dt);
	CNoteEdit& GetEditor();

	DECLARE_WND_CLASS(NOTE_WND_CLASS_NAME)

	BEGIN_MSG_MAP_EX(CNoteWnd)
		try
		{
			MSG_WM_CREATE(OnCreate)
			MSG_WM_DESTROY(OnDestroy)
			MESSAGE_HANDLER_EX(WM_INITNOTE, OnInitNote)
			MSG_WM_NCHITTEST(OnNcHittest)
 			MSG_WM_NCPAINT(OnNcPaint)
			MSG_WM_PAINT(OnPaint)
			MSG_WM_ACTIVATE(OnActivate)
			MSG_WM_ERASEBKGND(OnErasebkgnd)
			MSG_WM_GETMINMAXINFO(OnGetMinMaxInfo)
			MSG_WM_SIZE(OnSize)
			MSG_WM_KEYDOWN(OnKeyDown)
			MSG_WM_SETFOCUS(OnFocus)
			MSG_WM_KILLFOCUS(OnKillFocus)
			MSG_WM_MOVE(OnMove)
			MSG_WM_CTLCOLORSTATIC(OnCtlColorStatic)
			MSG_WM_CTLCOLOREDIT(OnCtlColorEdit)
			MSG_WM_INITMENUPOPUP(OnInitMenuPopup)
			COMMAND_ID_HANDLER_EX(ID_CLIPBRD_COPY, OnCopyToClipboard);
			COMMAND_ID_HANDLER_EX(ID_PASTE, OnPaste);
			COMMAND_ID_HANDLER_EX(ID_CLOSEALL, OnNoteCloseAll)
			COMMAND_ID_HANDLER_EX(ID_CLOSEALLBUTTHIS, OnNoteCloseAllButThis)
			COMMAND_ID_HANDLER_EX(ID_CLOSE, OnNoteClose)
			COMMAND_ID_HANDLER_EX(ID_DELETE, OnNoteDelete)
			NOTIFY_CODE_HANDLER_EX(EN_LINK, OnLink)
			CHAIN_COMMANDS_MEMBER(m_edit)
		} 
		CATCH_ALL_ERRORS(m_hWnd)
	END_MSG_MAP_EX()


	virtual void OnFinalMessage(HWND hWnd);

	LRESULT OnCreate(LPCREATESTRUCT lParam);
	void OnDestroy();
	LRESULT OnInitNote(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnNcHittest(CPoint pt);
	void OnPaint(HDC hdc);
	void OnActivate(UINT nState, BOOL bMinimized, HWND hWndOther);
	void OnNcPaint(HRGN wParam);
	BOOL OnErasebkgnd(HDC hdc) { return TRUE; }
	void OnGetMinMaxInfo(LPMINMAXINFO lParam);
	void OnSize(UINT wParam, CSize sz);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnFocus(HWND hWnd);
	void OnKillFocus(CWindow wndFocus);
	void OnMove(CPoint pt);
	HBRUSH OnCtlColorStatic(CDCHandle dc, CStatic wndStatic);
	HBRUSH OnCtlColorEdit(CDCHandle dc, CEdit edit);
	void OnInitMenuPopup(CMenuHandle menuPopup, UINT nIndex, BOOL bSysMenu);
	void OnCopyToClipboard(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnPaste(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnNoteCloseAll(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnNoteCloseAllButThis(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnNoteClose(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnNoteDelete(UINT uNotifyCode, int nID, CWindow wndCtl);
	LRESULT OnLink(LPNMHDR pnmh);
private:
	enum DownRegion
	{
		drNoWhere,
		drOnCloseButton,
		drOnCaption,
	};

	CRect GetIconRect();
	CRect GetCaptionRect();
	CRect GetBottomRightRect();
	CRect GetCloseButtonRect();
	CRect GetClientRect();
	CMenuHandle AdjustSystemMenu();
	void StoreNote();
	void DrawStatusBar(CDC& dc);
	static CBrush m_hBgBrush;
	static CIcon m_hIcon;
	static CIcon m_hIconSm;
	static CPen m_hPen;
	static CFont m_hStatusFont;

//	CToolTipCtrl m_tooltip;
	CNoteEdit m_edit;
	CEdit m_editCreated;
	CStaticSysIcon m_icon;
	CBitmapButton m_btnClose;
		
	int m_nNoteId;
	BOOL m_bSaveError;
	BOOL m_bPosChanged;
	time_t m_dtCreated;
	time_t m_dtModified;
	time_t m_dtDeleted;
};