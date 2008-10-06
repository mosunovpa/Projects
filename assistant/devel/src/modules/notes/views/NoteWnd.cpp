/**
 * \file NoteWnd.cpp
 *
 * \since 2007/01/09
 * \author pavel
 */

#include "stdafx.h"
#include "NoteWnd.h"
#include "NotesModuleImpl.h"

const INT s_nCaptionSize = 16;
const INT s_nCornerSize = 14;
const INT s_nStatusBarSize = 15;

CBrush CNoteWnd::m_hBgBrush = CreateSolidBrush(RGB(255, 255, 204));
CIcon CNoteWnd::m_hIcon = ::LoadIcon(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_NOTES));
CIcon CNoteWnd::m_hIconSm = ::LoadIcon(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_NOTES_SM));
CPen CNoteWnd::m_hPen = ::CreatePen(PS_SOLID, 1, RGB(0,0,0));

/**
 Constructor
 */
CNoteWnd::CNoteWnd(int nParentId, int nNoteId /*= 0*/) 
:	m_drDownWas(drNoWhere), 
	m_bActive(FALSE), 
	m_bCaptured(FALSE),
	m_nParentId(nParentId),
	m_nNoteId(nNoteId)
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
	delete this;
}

/**
Draw close button
*/
void CNoteWnd::DrawCloseButton(CDC& dc, BOOL bDown /*= FALSE*/)
{
	CRect rcCloseButton = GetCloseButtonRect();
	dc.FillRect(&rcCloseButton, 
		m_bActive? GetSysColorBrush(COLOR_ACTIVECAPTION) : GetSysColorBrush(COLOR_INACTIVECAPTION));

	int nDownOffset = bDown ? 1 : 0;
	CPen penWhite;
	penWhite.CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
	HPEN hOldPen = dc.SelectPen(penWhite);
	dc.MoveTo(rcCloseButton.left + 3 + nDownOffset, 3 + nDownOffset);
	dc.LineTo(rcCloseButton.right - 5 + nDownOffset, rcCloseButton.bottom - 5 + nDownOffset);
	dc.MoveTo(rcCloseButton.left + 3 + nDownOffset, rcCloseButton.bottom - 5 + nDownOffset);
	dc.LineTo(rcCloseButton.right - 5 + nDownOffset, 3 + nDownOffset);
	dc.SelectPen(hOldPen);

}

/**
WM_CREATE
*/
LRESULT CNoteWnd::OnCreate(LPCREATESTRUCT lParam)
{
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIconSm, FALSE);

	_tstring sTooltip = RESSTR(ID_CLOSE);
	m_tooltip.Create(m_hWnd, rcDefault, NULL, WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP, WS_EX_TOPMOST);
	m_tooltip.AddTool(&CToolInfo( TTF_SUBCLASS, m_hWnd, 1, &(GetCloseButtonRect()), &sTooltip[0]));
	m_tooltip.SetMaxTipWidth(300);
	m_tooltip.Activate(TRUE);

	m_edit.Create(m_hWnd, NULL, NULL, 
		WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | ES_MULTILINE | WS_VSCROLL, 
		WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY, 2);
	m_edit.SetBackgroundColor(RGB(255, 255, 204));

	CHARFORMAT cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_SIZE | CFM_BOLD | CFM_FACE
		/*
				   | CFM_CHARSET
				   | CFM_COLOR | CFM_ITALIC 
				   | CFM_OFFSET | CFM_PROTECTED 
				   | CFM_STRIKEOUT | CFM_UNDERLINE
				   */
		;
	m_edit.GetDefaultCharFormat(cf);
	cf.dwEffects = 0;
//	cf.yHeight = 165; // -11 (8)
	lstrcpy(cf.szFaceName, _T("MS Shell Dlg"));
	m_edit.SetDefaultCharFormat(cf);

	RECT rc = {0, 0, 200, 165 };
	SetWindowPos(NULL, &rc, SWP_NOZORDER | SWP_NOMOVE);

	CNotesModuleImpl::Get().GetMainApp()->AddItemWindow(m_hWnd);

	m_edit.SetFocus();

	return 0;
}

/**
WM_DESTROY
*/
void CNoteWnd::OnDestroy()
{
	CString csContent;
	m_edit.GetWindowText(csContent);

	CString csName = strutils::ExtractCaption(csContent).c_str();
	if (m_nNoteId > 0)
	{
		if (csName.IsEmpty())
		{
			CNotesModuleImpl::Get().GetMainApp()->DeleteItems(&m_nNoteId, 1);
		}
		else
		{
			CNotesModuleImpl::Get().GetMainApp()->StoreItem(m_nNoteId, csName, csContent);
		}
	}
	else if (!csName.IsEmpty())
	{
		CNotesModuleImpl::Get().GetMainApp()->NewItem(CNotesModuleImpl::Get().GetId(), m_nParentId, csName, csContent);
	}

	CNotesModuleImpl::Get().GetMainApp()->DeleteItemWindow(m_hWnd);
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
	if (GetIconRect().PtInRect(pt))
	{
		return HTSYSMENU;
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

		// Draw caption
		CRect rcCaption = GetCaptionRect();
		memDc.FillRect(&rcCaption, 
			m_bActive? GetSysColorBrush(COLOR_ACTIVECAPTION) : GetSysColorBrush(COLOR_INACTIVECAPTION));

		// Draw icon
		memDc.DrawIconEx(0, 0, GetIcon(FALSE), s_nCaptionSize, s_nCaptionSize);

		// Draw close button
		DrawCloseButton(memDc);

		// Draw the gripper
		
		HPEN hOldPen = memDc.SelectPen(m_hPen);

		memDc.MoveTo(rcClient.Width() - s_nStatusBarSize, rcClient.Height());
		memDc.LineTo(rcClient.Width(), rcClient.Height() - s_nStatusBarSize);
		memDc.MoveTo(rcClient.Width() - (s_nStatusBarSize - 4), rcClient.Height());
		memDc.LineTo(rcClient.Width(), rcClient.Height() - (s_nStatusBarSize - 4));
		memDc.MoveTo(rcClient.Width() - (s_nStatusBarSize - 8), rcClient.Height());
		memDc.LineTo(rcClient.Width(), rcClient.Height() - (s_nStatusBarSize - 8));
		memDc.MoveTo(rcClient.Width() - (s_nStatusBarSize - 12), rcClient.Height());
		memDc.LineTo(rcClient.Width(), rcClient.Height() - (s_nStatusBarSize - 12));

		// Draw the horisontal line
		memDc.MoveTo(0, rcClient.Height() - s_nStatusBarSize);
		memDc.LineTo(rcClient.Width(), rcClient.Height() - s_nStatusBarSize);

		memDc.SelectPen(hOldPen);
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
void CNoteWnd::OnLButtonDown(UINT wParam, CPoint point)
{
	if (GetCloseButtonRect().PtInRect(point))
	{
		m_drDownWas = drOnCloseButton;
		CClientDC dc(m_hWnd);
		DrawCloseButton(dc, TRUE);
		SetCapture();
		m_bCaptured = TRUE;
	}
	SetMsgHandled(FALSE);
}

/**
WM_LBUTTONUP
*/
void CNoteWnd::OnLButtonUp(UINT wParam, CPoint point)
{
	if (GetCloseButtonRect().PtInRect(point) && m_drDownWas == drOnCloseButton)
	{
		SendMessage(WM_CLOSE);
	}
	m_drDownWas = drNoWhere;
	if (m_bCaptured)
	{
		ReleaseCapture();
		m_bCaptured = FALSE;
	}
	SetMsgHandled(FALSE);
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
	m_tooltip.SetToolRect(m_hWnd, 1, &(GetCloseButtonRect()));
	m_edit.MoveWindow(&(GetClientRect()), TRUE);
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

