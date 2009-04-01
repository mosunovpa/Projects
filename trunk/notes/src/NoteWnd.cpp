/**
 * \file NoteWnd.cpp
 *
 * \since 2007/01/09
 * \author pavel
 */

#include "stdafx.h"
#include <boost/algorithm/string/trim.hpp>
#include "NoteWnd.h"
#include "resource.h"
#include "atlwinmisc.h"
#include "resutils.h"
#include "Application.h"
#include "guiutils.h"
#include "dateutils.h"
#include "Clipboard.h"

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
:	m_nNoteId(nNoteId),
	m_bPosChanged(FALSE),
	m_dtCreated(0),
	m_dtModified(0),
	m_dtDeleted(0)
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

	/*
	_tstring sTooltip = RESSTR(IDS_CLOSE);
	m_tooltip.Create(m_hWnd, rcDefault, NULL, WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP, WS_EX_TOPMOST);
	m_tooltip.AddTool(&CToolInfo( TTF_SUBCLASS, m_hWnd, 1, &(GetCloseButtonRect()), &sTooltip[0]));
	m_tooltip.SetMaxTipWidth(300);
*/

	m_btnClose.Create(m_hWnd, NULL, NULL, WS_CHILD | WS_VISIBLE, 0, ID_CLOSE);
	m_btnClose.SetBitmapButtonExtendedStyle(BMPBTN_HOVER | BMPBTN_AUTOSIZE);
	CImageList il;
	CBitmap bmpClose;
	bmpClose.LoadBitmap(IDB_CLOSE_BTNS);
	il.Create(16, 16, ILC_COLOR24, 4, 0); //, RGB(0,0,0), IMAGE_BITMAP, 
	il.Add(bmpClose, RGB(0,0,0));
	m_btnClose.SetImageList(il); // il will be deleted in ~CBitmapButtonImpl()
	m_btnClose.SetImages(0,1,2,3);
	m_btnClose.SetToolTipText(RESSTR(IDS_CLOSE));

	m_editCreated.Create(m_hWnd, NULL, NULL, WS_CHILD | WS_VISIBLE | ES_READONLY);
 	m_editCreated.SetFont(m_hStatusFont);

	m_edit.Create(m_hWnd, NULL, NULL, 
		WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | ES_MULTILINE | WS_VSCROLL, 
		WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY, 20000);
	m_edit.SetBackgroundColor(RGB(255, 255, 204));


	COptions::FontSize fs = CApplication::Get().GetOptions().GetFontSize();
	CHARFORMAT cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_SIZE | CFM_BOLD | CFM_FACE;
	m_edit.GetDefaultCharFormat(cf);
	cf.yHeight = (fs == COptions::FS_SMALL ? 160 : (fs == COptions::FS_MEDIUM ? 200 : 240));
	cf.dwEffects = 0;
	lstrcpy(cf.szFaceName, _T("MS Shell Dlg"));
	m_edit.SetDefaultCharFormat(cf);

	// format changing lead to increase undo queue

	PARAFORMAT pf;
	ZeroMemory(&pf, sizeof(PARAFORMAT));
	pf.cbSize = sizeof(PARAFORMAT);
	pf.dwMask = PFM_OFFSETINDENT;
	pf.dxStartIndent = 100;
	m_edit.SetParaFormat(pf);

	m_edit.SetOleCallback(&m_edit.m_OleCallback);
	m_edit.SetEventMask(ENM_LINK);
	m_edit.SetAutoURLDetect();
	m_edit.SetFocus();

	PostMessage(WM_INITNOTE);

	return 0;
}

/**
WM_DESTROY
*/
void CNoteWnd::OnDestroy()
{
	if (GetText().empty())
	{
		CApplication::Get().DeleteFromStorage(GetId());
	}
	else
	{
		StoreNote();
	}
}

/* WM_INITNOTE */
LRESULT CNoteWnd::OnInitNote(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	m_bPosChanged = FALSE;
	m_edit.SetModify(FALSE);
	m_edit.EmptyUndoBuffer();
	return 0;
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
		DrawStatusBar(memDc);
	}
}

/**
 WM_ACTIVATE
 */
void CNoteWnd::OnActivate(UINT nState, BOOL bMinimized, HWND hWndOther)
{
	m_btnClose.SetImages(nState == 0 ? 3 : 0);
	m_btnClose.Invalidate(FALSE);
   	m_btnClose.UpdateWindow();
	Invalidate(FALSE);
	UpdateWindow();

	if (nState == WA_INACTIVE)
	{
		StoreNote();
	}

}

/**
 WM_GETMINMAXINFO
 */
void CNoteWnd::OnGetMinMaxInfo(LPMINMAXINFO lParam)
{
	lParam->ptMinTrackSize = CPoint(160, s_nCaptionSize + 46);
}

/**
 WM_SIZE
 */
void CNoteWnd::OnSize(UINT wParam, CSize sz)
{
	m_btnClose.MoveWindow(&GetCloseButtonRect(), TRUE);

	CRect rc;
	::GetClientRect(m_hWnd, &rc);
	CRect rcCreated(0, rc.bottom - s_nStatusBarSize + 1, 135, rc.bottom);
	m_editCreated.MoveWindow(&rcCreated, TRUE);

//	m_tooltip.SetToolRect(m_hWnd, 1, &(GetCloseButtonRect()));
	m_edit.MoveWindow(&(GetClientRect()), TRUE);

	m_bPosChanged = TRUE;
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
	m_bPosChanged = TRUE;
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
Store note if text is not empty
*/
void CNoteWnd::StoreNote()
{
	if (!GetText().empty())
	{
		if (m_edit.GetModify()) // save all if note content has been changed
		{
			m_dtModified = dateutils::GetCurrentDate();
			m_nNoteId = CApplication::Get().SaveNote(this, CApplication::NM_ALL);
		}
		else if (m_bPosChanged) // only position changed
		{
			CApplication::Get().SaveNote(this, CApplication::NM_POS);
		}
		m_edit.SetModify(FALSE);
		m_bPosChanged = FALSE;
	}
}

/**/
int CNoteWnd::GetId() const
{
	return m_nNoteId;
}

/**/
_tstring CNoteWnd::GetText() const
{
	int len = m_edit.GetWindowTextLength() + 1;
	_tstring s;
	s.resize(len);
	m_edit.GetWindowText(&s[0],len);
	boost::trim(s);
	if (s[0] == 0)
	{
		s.clear();
	}
	return s;
}

/**/
void CNoteWnd::SetId( int id )
{
	m_nNoteId = id;
}

/**/
void CNoteWnd::SetText(LPCTSTR text)
{
	m_edit.SetWindowText(text);
}

/**/
time_t CNoteWnd::GetCreatedDate() const
{
	return m_dtCreated;
}

/**/
void CNoteWnd::SetCreatedDate(time_t dt)
{
	m_dtCreated = dt;
	if (dt != 0)
	{
		_tstring sDate = dateutils::ToString(dt, _T("%#d %b %Y, %H:%M"));
		m_editCreated.SetWindowText(RESSTR_FMT(IDS_CREATED_FRM, sDate.c_str()).c_str());
	}
}

/**/
time_t CNoteWnd::GetModifiedDate() const
{
	return m_dtModified;
}

/**/
void CNoteWnd::SetModifiedDate(time_t dt)
{
	m_dtModified = dt;
}

CNoteEdit& CNoteWnd::GetEditor()
{
	return m_edit;
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
	if (GetKeyState(VK_CONTROL) & 0x8000)
	{
		CApplication::Get().CloseAllNotes(this); // close all but this
	}
	PostMessage(WM_CLOSE);
}

/* ID_CLIPBRD_COPY */
void CNoteWnd::OnCopyToClipboard(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if (!GetText().empty())
	{
 		long nStart, nEnd;
 		m_edit.GetSel(nStart, nEnd);
		m_edit.SetSel(0, -1);
		m_edit.Copy();
		m_edit.SetSel(nStart, nEnd);
	}
}

/* ID_DELETE */
void CNoteWnd::OnNoteDelete( UINT uNotifyCode, int nID, CWindow wndCtl )
{
	try
	{
		StoreNote(); // save content before deleting for undelete
	}
	CATCH_ALL_ERRORS(m_hWnd)

	SetText(_T(""));
	PostMessage(WM_CLOSE);
}


/* ID_CLOSEALL */
void CNoteWnd::OnNoteCloseAll(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	CApplication::Get().CloseAllNotes(this); // close all but this
	PostMessage(WM_CLOSE); // close this
}

/* ID_CLOSEALLBUTTHIS */
void CNoteWnd::OnNoteCloseAllButThis(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	CApplication::Get().CloseAllNotes(this); // close all but this
}

/* ID_PASTE */
void CNoteWnd::OnPaste(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	m_edit.Paste();
}

/**/
void CNoteWnd::OnInitMenuPopup(CMenuHandle menuPopup, UINT nIndex, BOOL bSysMenu)
{
	menuutils::SetMenuItemEnable(menuPopup, ID_EDIT_UNDO, m_edit.CanUndo());
	menuutils::SetMenuItemEnable(menuPopup, ID_EDIT_REDO, m_edit.CanRedo());
	menuutils::SetMenuItemEnable(menuPopup, ID_EDIT_CUT, m_edit.CanCut());
	menuutils::SetMenuItemEnable(menuPopup, ID_EDIT_COPY, m_edit.CanCopy());
	menuutils::SetMenuItemEnable(menuPopup, ID_EDIT_PASTE, m_edit.CanPaste());
	menuutils::SetMenuItemEnable(menuPopup, ID_EDIT_CLEAR, m_edit.CanClear());
	menuutils::SetMenuItemEnable(menuPopup, ID_EDIT_SELECT_ALL, m_edit.CanSelectAll());
}

/**/
LRESULT CNoteWnd::OnLink(LPNMHDR pnmh)
{
	ENLINK* pLinkInfo = (ENLINK*)pnmh;
	if (pLinkInfo->msg == WM_LBUTTONUP)
	{
		_tstring txt;
		txt.resize(pLinkInfo->chrg.cpMax - pLinkInfo->chrg.cpMin + 1);
		m_edit.GetTextRange(pLinkInfo->chrg.cpMin, pLinkInfo->chrg.cpMax, &txt[0]);
		ShellExecute(NULL, NULL, txt.c_str(), NULL, NULL, SW_SHOW);
	}
	return 0;
}


time_t CNoteWnd::GetDeletedDate() const
{
	return m_dtDeleted;

}

void CNoteWnd::SetDeletedDate( time_t dt )
{
	m_dtDeleted = dt;
}
