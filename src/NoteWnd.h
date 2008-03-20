/**
 * \file NoteWnd.h
 *
 * \since 2007/01/09
 * \author pavel
 */

#pragma once

//////////////////////////////////////////////////////////////////////////
//
class CNoteEdit : public CWindowImpl<CNoteEdit, CRichEditCtrl>,
                    public CRichEditCommands<CNoteEdit>
{
public:
	DECLARE_WND_SUPERCLASS(NULL, CRichEditCtrl::GetWndClassName())
    BEGIN_MSG_MAP(CNoteEdit)
		MESSAGE_HANDLER_EX(WM_KEYDOWN, OnKeyDown)
        CHAIN_MSG_MAP_ALT(CRichEditCommands<CNoteEdit>, 1)
    END_MSG_MAP()

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

};

//////////////////////////////////////////////////////////////////////////
//

class CNoteWnd : public CWindowImpl<CNoteWnd>
{
public:
	CNoteWnd(int nNoteId = 0);
	virtual ~CNoteWnd();

	int GetId() const;
	CString GetText() const;

	DECLARE_WND_CLASS(_T("NoteWnd"))

	BEGIN_MSG_MAP_EX(CNoteWnd)
		try
		{
			MSG_WM_CREATE(OnCreate)
			MSG_WM_DESTROY(OnDestroy)
			MSG_WM_NCHITTEST(OnNcHittest)
 			MSG_WM_NCPAINT(OnNcPaint)
			MSG_WM_PAINT(OnPaint)
			MSG_WM_ACTIVATE(OnActivate)
			MSG_WM_ERASEBKGND(OnErasebkgnd)
			MSG_WM_LBUTTONDOWN(OnLButtonDown)
			MSG_WM_LBUTTONUP(OnLButtonUp)
			MSG_WM_MOUSEMOVE(OnMouseMove)
			MSG_WM_GETMINMAXINFO(OnGetMinMaxInfo)
			MSG_WM_SIZE(OnSize)
			MSG_WM_KEYDOWN(OnKeyDown)
			MSG_WM_SETFOCUS(OnFocus)
			MSG_WM_KILLFOCUS(OnKillFocus)
			MSG_WM_MOVE(OnMove)
		} 
		CATCH_ALL_ERRORS(m_hWnd)
	END_MSG_MAP_EX()

	virtual void OnFinalMessage(HWND hWnd);

	LRESULT OnCreate(LPCREATESTRUCT lParam);
	void OnDestroy();
	LRESULT OnNcHittest(CPoint pt);
	void OnPaint(HDC hdc);
	void OnActivate(UINT nState, BOOL bMinimized, HWND hWndOther);
	void OnNcPaint(HRGN wParam);
	LRESULT OnErasebkgnd(HDC hdc) { return TRUE; }
	void OnLButtonDown(UINT wParam, CPoint point);
	void OnLButtonUp(UINT wParam, CPoint point);
	void OnMouseMove(UINT wParam, CPoint point);
	void OnGetMinMaxInfo(LPMINMAXINFO lParam);
	void OnSize(UINT wParam, CSize sz);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnFocus(HWND hWnd);
	void OnKillFocus(CWindow wndFocus);
	void OnMove(CPoint pt);

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
	void DrawCloseButton(CDC& dc, BOOL bDown = FALSE);
	void AdjustSystemMenu();
	void StoreNote();
	void RemoveNote();

	static CBrush m_hBgBrush;
	static CIcon m_hIcon;
	static CIcon m_hIconSm;
	static CPen m_hPen;

	CToolTipCtrl m_tooltip;
	CNoteEdit m_edit;
		
	BOOL m_bActive;
	DownRegion m_drDownWas;

	BOOL m_bCaptured;
	int m_nNoteId;
};