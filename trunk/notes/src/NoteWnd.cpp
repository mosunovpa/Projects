/**
 * \file NoteWnd.cpp
 *
 * \since 2007/01/09
 * \author pavel
 */

#include "stdafx.h"
#include "NoteWnd.h"
#include "resource.h"
#include "atlwinmisc.h"
#include "resutils.h"
#include "Application.h"
#include "guiutils.h"

const INT s_nCaptionSize = 16;
const INT s_nCornerSize = 14;
const INT s_nStatusBarSize = 15;

CBrush CNoteWnd::m_hBgBrush = CreateSolidBrush(RGB(255, 255, 204));
CIcon CNoteWnd::m_hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME),
								  IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
CIcon CNoteWnd::m_hIconSm = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_NOTES_SM),
								  IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
CPen CNoteWnd::m_hPen = ::CreatePen(PS_SOLID, 1, RGB(0,0,0));

CFont CNoteWnd::m_hStatusFont = CFontHandle().CreatePointFont(80, _T("MS Shell Dlg"));

/**
 Constructor
 */
CNoteWnd::CNoteWnd(int nNoteId /*= 0*/) 
:	m_drDownWas(drNoWhere), 
	m_bActive(FALSE), 
	m_bCaptured(FALSE),
	m_nNoteId(nNoteId),
	m_bSaveError(FALSE)
{
}

/**
 Destructor
 */
CNoteWnd::~CNoteWnd()
{
}

/**
 Returns icon rect
 */
CRect CNoteWnd::GetIconRect()
{
	return CRect(0, 0, s_nCaptionSize, s_nCaptionSize);
}

/**
 Returns caption coordinates
 */
CRect CNoteWnd::GetCaptionRect()
{
	CClientRect rc(m_hWnd);
	rc.left = s_nCaptionSize + 2;
	rc.bottom = s_nCaptionSize;
	rc.right -= (s_nCaptionSize + 3);
	return rc;
}

CRect CNoteWnd::GetClientRect()
{
	CClientRect rc(m_hWnd);
	rc.top += s_nCaptionSize + 4;
	rc.bottom -= (s_nStatusBarSize + 2);
	return rc;
}


/**
 Returns close button rect
 */
CRect CNoteWnd::GetCloseButtonRect()
{
	CClientRect rc(m_hWnd);
	rc.left = rc.right - s_nCaptionSize;
	rc.bottom = s_nCaptionSize;
	return rc;
}

/**
 Returns bottom right corner for window sizing
 */
CRect CNoteWnd::GetBottomRightRect()
{
	CClientRect rc(m_hWnd);
	rc.left = rc.right - s_nCornerSize;
	rc.top = rc.bottom - s_nCornerSize;
	return rc;
}

/**
 Last message - destroy object
 */
void CNoteWnd::OnFinalMessage(HWND hWnd)
{
	CApplication::Get().OnNoteClosed(this);
}

/**
Draw close button
*/
void CNoteWnd::DrawCloseButton(CDC& dc, BOOL bDown /*= FALSE*/)
{
	CBitmap bmp;
	if (bDown)
	{
		bmp.LoadBitmap(IDB_CLOSE_PRESS);
	}
	else
	{
		bmp.LoadBitmap(IDB_CLOSE);
	}
	guiutils::DrawBitmap(dc, bmp, GetCloseButtonRect());
}

/* draw status bar */
void CNoteWnd::DrawStatusBar(CDC& dc)
{
	

	HPEN hOldPen, hPenLine;

	CRect rectDlg;
	::GetClientRect(m_hWnd,&rectDlg);

	// Create a grey pen
	hPenLine = ::CreatePen(PS_SOLID, 1, RGB(125,125,125));
	ATLASSERT(hPenLine);	

	// Select the new pen into the device context
	hOldPen = (HPEN)::SelectObject(dc, hPenLine);
	ATLASSERT(hOldPen);
	ATLASSERT(hOldPen != HGDI_ERROR);

	// Draw the gripper
	int offset = s_nStatusBarSize;
	while (offset > 0)
	{
		dc.MoveTo(rectDlg.Width() - offset, rectDlg.Height());
		dc.LineTo(rectDlg.Width(), rectDlg.Height() - offset);
		offset -= 4;
	}

	// Draw the horizontal line
// 	dc.MoveTo(0, rectDlg.Height() - s_nStatusBarSize);
// 	dc.LineTo(rectDlg.Width(), rectDlg.Height() - s_nStatusBarSize);

	// Clean up
	::SelectObject(dc, hOldPen);
	::DeleteObject(hPenLine);
}

/**
WM_CREATE
*/
LRESULT CNoteWnd::OnCreate(LPCREATESTRUCT lParam)
{
//	SetIcon(m_hIcon, TRUE);
//	SetIcon(m_hIconSm, FALSE);

	m_icon.Create(m_hWnd, GetIconRect(), NULL, WS_CHILD|WS_VISIBLE|SS_ICON|SS_CENTERIMAGE|SS_NOTIFY);
	m_icon.SetIcon(m_hIconSm);


	AdjustSystemMenu();

	_tstring sTooltip = RESSTR(IDS_CLOSE);
	m_tooltip.Create(m_hWnd, rcDefault, NULL, WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP, WS_EX_TOPMOST);
	m_tooltip.AddTool(&CToolInfo( TTF_SUBCLASS, m_hWnd, 1, &(GetCloseButtonRect()), &sTooltip[0]));
	m_tooltip.SetMaxTipWidth(300);


	m_editCreated.Create(m_hWnd, NULL, NULL, WS_CHILD | WS_VISIBLE | ES_READONLY);
	m_editCreated.SetWindowText(_T("Created"));

 	m_editCreated.SetFont(m_hStatusFont);

	m_edit.Create(m_hWnd, NULL, NULL, 
		WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | ES_MULTILINE | WS_VSCROLL, 
		WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR /*| WS_EX_NOPARENTNOTIFY*/, 2);
	m_edit.SetBackgroundColor(RGB(255, 255, 204));


	CHARFORMAT cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_SIZE | CFM_BOLD | CFM_FACE;
	m_edit.GetDefaultCharFormat(cf);
	cf.dwEffects = 0;
	lstrcpy(cf.szFaceName, _T("MS Shell Dlg"));
	m_edit.SetDefaultCharFormat(cf);

	// format changing lead to increase undo queue

	//PARAFORMAT pf;
	//ZeroMemory(&pf, sizeof(PARAFORMAT));
	//pf.cbSize = sizeof(PARAFORMAT);
	//pf.dwMask = PFM_OFFSETINDENT;
	//pf.dxStartIndent = 100;
	//m_edit.SetParaFormat(pf);

	m_edit.SetOleCallback(&m_edit.m_OleCallback);

	m_edit.SetFocus();

	return 0;
}

/**
WM_DESTROY
*/
void CNoteWnd::OnDestroy()
{
	if (GetText().IsEmpty())
	{
		RemoveNote();
	}
	else
	{
		StoreNote();
	}
}

/**
WM_NCHITTEST
*/
LRESULT CNoteWnd::OnNcHittest(CPoint pt)
{
	ScreenToClient(&pt);
	if (GetCaptionRect().PtInRect(pt))
	{
		return HTCAPTION;
	}
	if (GetBottomRightRect().PtInRect(pt))
	{
		return HTBOTTOMRIGHT;
	}
	SetMsgHandled(FALSE);
	return 0;
}

/**
 WM_NCPAINT
 */
void CNoteWnd::OnNcPaint(HRGN wParam)
{
	HDC hdc;
	hdc = GetDCEx(wParam, DCX_WINDOW | DCX_INTERSECTRGN | DCX_CACHE | DCX_CLIPSIBLINGS);

	CWindowRect rcWindow(m_hWnd);
	rcWindow.OffsetRect(-rcWindow.left, -rcWindow.top);

	CClientRect rcClient(m_hWnd);
	rcClient.OffsetRect(GetSystemMetrics(SM_CXSIZEFRAME), GetSystemMetrics(SM_CYSIZEFRAME));

	CRgn rgClient = ::CreateRectRgnIndirect(&rcClient);
	CRgn rgWindow = ::CreateRectRgnIndirect(&rcWindow);
	rgWindow.CombineRgn(rgClient, RGN_DIFF);
	::SelectClipRgn(hdc, rgWindow);

	{
		CMemoryDC memDc(hdc, rcWindow);
		HBRUSH hOldBrush = memDc.SelectBrush(m_hBgBrush);
		HPEN hOldPen = memDc.SelectPen(m_hPen);
		memDc.Rectangle(&rcWindow);
		memDc.SelectPen(hOldPen);
		memDc.SelectBrush(hOldBrush);
	}	
	ReleaseDC(hdc);
}

/**
 WM_PAINT
 */
void CNoteWnd::OnPaint(HDC hdc)
{
	CPaintDC dc(m_hWnd);
	{
		CClientRect rcClient(m_hWnd);
		CMemoryDC memDc(dc, rcClient);
		memDc.FillRect(&rcClient, m_hBgBrush);

		// Draw icon
//		memDc.DrawIconEx(0, 0, GetIcon(FALSE), s_nCaptionSize, s_nCaptionSize);

		// Draw close button
		DrawCloseButton(memDc);

		DrawStatusBar(memDc);
	}
}

/**
 WM_ACTIVATE
 */
void CNoteWnd::OnActivate(UINT nState, BOOL bMinimized, HWND hWndOther)
{
 	m_bActive = nState;
	Invalidate(FALSE);
   	UpdateWindow();
}

/**
 WM_LBUTTONDOWN
 */
void CNoteWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rc = GetCloseButtonRect();
	if (rc.PtInRect(point))
	{
		m_drDownWas = drOnCloseButton;
		CClientDC dc(m_hWnd);
		DrawCloseButton(dc, TRUE);
		SetCapture();
		m_bCaptured = TRUE;
	}
}

/**
WM_LBUTTONUP
*/
void CNoteWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (GetCloseButtonRect().PtInRect(point) && m_drDownWas == drOnCloseButton)
	{
		if (GetKeyState(VK_CONTROL) & 0x8000)
		{
			CApplication::Get().CloseAllNotes(this); // close all but this
		}
		SendMessage(WM_CLOSE); // close this
	}
	m_drDownWas = drNoWhere;
	if (m_bCaptured)
	{
		ReleaseCapture();
		m_bCaptured = FALSE;
	}
}

/**
 WM_MOUSEMOVE
 */
void CNoteWnd::OnMouseMove(UINT wParam, CPoint point)
{
	if (m_drDownWas == drOnCloseButton)
	{
		CClientDC dc(m_hWnd);
		DrawCloseButton(dc, GetCloseButtonRect().PtInRect(point));
	}
}

/**
 WM_GETMINMAXINFO
 */
void CNoteWnd::OnGetMinMaxInfo(LPMINMAXINFO lParam)
{
	lParam->ptMinTrackSize = CPoint(150, s_nCaptionSize + 46);
}

/**
 WM_SIZE
 */
void CNoteWnd::OnSize(UINT wParam, CSize sz)
{
	CRect rc;
	::GetClientRect(m_hWnd, &rc);
	CRect rcCreated(0, rc.bottom - s_nStatusBarSize + 1, 100, rc.bottom);
	m_editCreated.MoveWindow(&rcCreated, TRUE);

	m_tooltip.SetToolRect(m_hWnd, 1, &(GetCloseButtonRect()));
	m_edit.MoveWindow(&(GetClientRect()), TRUE);
	m_edit.SetModify(TRUE);
}

/**
 WM_KEYDOWN
 */
void CNoteWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_ESCAPE)
	{
		SendMessage(WM_CLOSE);
	}
	else
	{
		SetMsgHandled(FALSE);
	}
}

/*
WM_SETFOCUS
*/
void CNoteWnd::OnFocus(HWND hWnd)
{
	m_edit.SetFocus();
	SetMsgHandled(FALSE);
}

/*
WM_MOVE
*/
void CNoteWnd::OnMove(CPoint pt)
{
	m_edit.SetModify(TRUE);
}

/*
WM_CTLCOLORSTATIC
*/
HBRUSH CNoteWnd::OnCtlColorStatic(CDCHandle dc, CStatic wndStatic)
{
	if (wndStatic.m_hWnd == m_editCreated.m_hWnd)
	{
		dc.SetBkColor(RGB(255, 255, 204));
		dc.SetTextColor(RGB(125, 125, 125));
	}
	return m_hBgBrush;
}

/* WM_CTLCOLOREDIT */
HBRUSH CNoteWnd::OnCtlColorEdit(CDCHandle dc, CEdit edit)
{
	return m_hBgBrush;
}

/*
WM_KILLFOCUS
*/
void CNoteWnd::OnKillFocus(CWindow wndFocus)
{
	try 
	{
		if (!m_bSaveError) // need for prevent recursion when error occurs
		{
			StoreNote();
		}
	}
	catch(...)
	{
		m_bSaveError = TRUE;
		throw;
	}
}

/*
Store note if text is not empty
*/
void CNoteWnd::StoreNote()
{
	if (!GetText().IsEmpty() && m_edit.GetModify())
	{
		m_nNoteId = CApplication::Get().SaveNote(this);
		m_edit.SetModify(FALSE);
	}
}

/**/
void CNoteWnd::RemoveNote()
{
	CApplication::Get().DeleteNote(this);
};

/**/
int CNoteWnd::GetId() const
{
	return m_nNoteId;
}

/**/
CString CNoteWnd::GetText() const
{
	CString s;
	m_edit.GetWindowText(s);
	return s.Trim();
}

/**/
void CNoteWnd::SetId( int id )
{
	m_nNoteId = id;
}

/**/
void CNoteWnd::SetText( CString const& text )
{
	m_edit.SetWindowText(text);
}

/**/
CMenuHandle CNoteWnd::AdjustSystemMenu()
{
	CMenuHandle menu = GetSystemMenu(FALSE);
	menu.DeleteMenu(SC_MINIMIZE, MF_BYCOMMAND);
	menu.DeleteMenu(SC_MAXIMIZE, MF_BYCOMMAND);
	menu.DeleteMenu(SC_RESTORE, MF_BYCOMMAND);
	return menu;
}

/* ID_CLOSE */
void CNoteWnd::OnNoteClose( UINT uNotifyCode, int nID, CWindow wndCtl )
{
	SendMessage(WM_CLOSE);
}

/* ID_DELETE */
void CNoteWnd::OnNoteDelete( UINT uNotifyCode, int nID, CWindow wndCtl )
{
	CSimpleDialog<IDD_DELETENOTECONFIRM> dlg;
	if (dlg.DoModal() == IDYES)
	{
		SetText(CString());
		SendMessage(WM_CLOSE);
	}
}

/* ID_CLOSEALL */
void CNoteWnd::OnNoteCloseAll(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	CApplication::Get().CloseAllNotes(this); // close all but this
	SendMessage(WM_CLOSE); // close this
}

/* ID_CLOSEALLBUTTHIS */
void CNoteWnd::OnNoteCloseAllButThis(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	CApplication::Get().CloseAllNotes(this); // close all but this
}

void CNoteWnd::OnInitMenuPopup(CMenu menuPopup, UINT nIndex, BOOL bSysMenu)
{
	menuutils::SetMenuItemEnable(menuPopup, ID_EDIT_UNDO, m_edit.CanUndo());
	menuutils::SetMenuItemEnable(menuPopup, ID_EDIT_REDO, m_edit.CanRedo());
	menuutils::SetMenuItemEnable(menuPopup, ID_EDIT_CUT, m_edit.CanCut());
	menuutils::SetMenuItemEnable(menuPopup, ID_EDIT_COPY, m_edit.CanCopy());
	menuutils::SetMenuItemEnable(menuPopup, ID_EDIT_PASTE, m_edit.CanPaste());
	menuutils::SetMenuItemEnable(menuPopup, ID_EDIT_CLEAR, m_edit.CanClear());
	menuutils::SetMenuItemEnable(menuPopup, ID_EDIT_SELECT_ALL, m_edit.CanSelectAll());
}