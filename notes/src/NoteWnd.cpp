/**
 * \file NoteWnd.cpp
 *
 * \since 2007/01/09
 * \author pavel
 */

#include "stdafx.h"
#include <boost/algorithm/string/trim.hpp>
#include "Application.h"
#include "NoteWnd.h"
#include "resource.h"
#include "atlwinmisc.h"
#include "resutils.h"
#include "guiutils.h"
#include "dateutils.h"
#include "Clipboard.h"
#include "NewLabelDialog.h"

#define RIGHT_ROLLUP_BTN

//const INT s_nCaptionSize = 16;
const INT s_nCornerSize = 14;
const INT s_nStatusBarSize = 15;

CBrush CNoteWnd::m_hBgBrush = CreateSolidBrush(RGB(255, 255, 204));
CIcon CNoteWnd::m_hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME),
								  IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
//CIcon CNoteWnd::m_hIconSm = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_NOTES_SM),
//								  IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
//CIcon CNoteWnd::m_hIconTrash = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_TRASH_SM),
//											   IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);

CPen CNoteWnd::m_hPen = ::CreatePen(PS_SOLID, 1, RGB(0,0,0));
CPen CNoteWnd::m_hGrayPen = ::CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
CFont CNoteWnd::m_hStatusFont = CFontHandle().CreatePointFont(80, _T("MS Shell Dlg"));

/**
 Constructor
 */
CNoteWnd::CNoteWnd(int nNoteId /*= 0*/) 
:	m_nNoteId(nNoteId),
	m_dtCreated(0),
	m_dtDeleted(0),
	m_bMinimized(FALSE),
//	m_icon(this),
	m_bInitialized(FALSE),
	m_bActive(FALSE),
	m_flagSave(NM_NONE),
	m_flagInit(NF_NONE),
	m_bPrevActive(FALSE)
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
/*
CRect CNoteWnd::GetIconRect()
{
	return CRect(0, 0, s_nCaptionSize, s_nCaptionSize);
}
*/
/**/

void CNoteWnd::ShowSystemMenu(CPoint pt)
{
	if (m_activeMenu.IsMenu())
	{
		return;
	}

	// Load the menu resource 
	if (!m_activeMenu.LoadMenu(IDR_NOTEMENU))
	{
		return;
	}
	CMenuHandle menuPopup;
	if (GetDeletedDate() == 0)
	{
		menuPopup = m_activeMenu.GetSubMenu(0);
		if (menuPopup.m_hMenu == NULL)
		{
			return;
		}
		CMenuHandle menuLabels = menuPopup.GetSubMenu(1);
		PopulateLabelMenu(menuLabels);
	}
	else
	{
		menuPopup = m_activeMenu.GetSubMenu(1);
		if (menuPopup.m_hMenu == NULL)
		{
			return;
		}
	}

	menuPopup.SetMenuDefaultItem(ID_CLOSE);

	if (CApplication::Get().GetOpenedNotesCount() == 1)
	{
		menuutils::SetMenuItemEnable(menuPopup, ID_CLOSEALL, CApplication::Get().GetOpenedNotesCount() > 1);
		menuutils::SetMenuItemEnable(menuPopup, ID_CLOSEALLBUTTHIS, CApplication::Get().GetOpenedNotesCount() > 1);
	}
	menuPopup.DeleteMenu(IsMinimized() ? ID_ROLLUP : ID_UNROLL, MF_BYCOMMAND);

	if (!menuPopup.TrackPopupMenu(TPM_LEFTALIGN|TPM_TOPALIGN|TPM_LEFTBUTTON,
		pt.x, pt.y, m_hWnd))
	{
		return;
	}
	m_activeMenu.DestroyMenu();
}

/**/
void CNoteWnd::ShowLabelMenu(CPoint pt)
{
	if (m_activeMenu.IsMenu())
	{
		return;
	}

	// Load the menu resource 
	if (!m_activeMenu.LoadMenu(IDR_NOTEMENU))
	{
		return;
	}
	CMenuHandle menuPopup = m_activeMenu.GetSubMenu(0);
	if (menuPopup.m_hMenu == NULL)
	{
		return;
	}
	CMenuHandle menuLabels = menuPopup.GetSubMenu(1);
	PopulateLabelMenu(menuLabels);

	if (!menuLabels.TrackPopupMenu(TPM_LEFTALIGN|TPM_TOPALIGN|TPM_LEFTBUTTON,
		pt.x, pt.y, m_hWnd))
	{
		return;
	}
	m_activeMenu.DestroyMenu();
}

/**/
void CNoteWnd::PopulateLabelMenu(CMenuHandle menuLabels)
{
	m_listLabels.clear();
	CApplication::Get().GetLabels(m_listLabels);
	if (!m_label.empty() && (m_flagSave & NM_LABEL))
	{
		m_listLabels.push_back(m_label);
		m_listLabels.sort();
		m_listLabels.unique();
	}
	if (!m_listLabels.empty())
	{
		menuLabels.AppendMenu(MF_SEPARATOR);
	}
	menuLabels.EnableMenuItem(ID_LABEL_CLEAR, GetLabel().empty() ? MF_GRAYED : MF_ENABLED);
	int nSelCmd = LABEL_CMD_FIRST;
	int pos = 1;
	for (std::list<_tstring>::iterator it = m_listLabels.begin();
		it != m_listLabels.end(); ++it)
	{
		int nCmd = CREATE_LABEL_CMD(pos);
		menuLabels.AppendMenu(MF_STRING, nCmd, it->c_str());
		if (*it == GetLabel())
		{
			nSelCmd = nCmd;
		}
		++pos;
	}
	menuLabels.CheckMenuRadioItem(LABEL_CMD_FIRST, LABEL_CMD_LAST, nSelCmd, MF_BYCOMMAND);
}


/**
 Returns caption coordinates
 */
CRect CNoteWnd::GetCaptionRect()
{
	int leftAlignIdx = GetButtonIndex(ID_SYSMENU);
	CRect rc;
	int cnt = GetButtonCount();
	if (cnt > 0) 
	{
		for (int i = 0; i < cnt; ++i)
		{
			POINT bp = GetButtonPos(i);
			SIZE bs = GetButtonSize(i);
			rc.top = bp.y;
			rc.bottom = bp.y + bs.cy;

			if (i < leftAlignIdx)
				rc.right = bp.x;
			else
				rc.left = bp.x + bs.cx + 5;
		}
	}
	return rc;
/*
	CClientRect rc(m_hWnd);
#ifdef RIGHT_ROLLUP_BTN
 	rc.left = s_nCaptionSize + 2;
 	rc.right -= (s_nCaptionSize + s_nCaptionSize + 3);
#else
	rc.left = s_nCaptionSize + s_nCaptionSize + 2;
	rc.right -= (s_nCaptionSize + 3);
#endif
	rc.bottom = s_nCaptionSize;
	return rc;
*/
}

/**/
int CNoteWnd::GetMinimizedHeight()
{
	int caption = GetSystemMetrics(SM_CYSMCAPTION);
	int border = GetSystemMetrics(SM_CYSIZEFRAME);
	return  caption + border * 2;
}

/**/
int CNoteWnd::GetMinimizedWidth()
{
	return 180;
}

/**/
CRect CNoteWnd::GetClientRect()
{
	CClientRect rc(m_hWnd);
//	rc.top += s_nCaptionSize + 4;
	rc.bottom -= (s_nStatusBarSize + 2);
	return rc;
}

/**/
CRect CNoteWnd::GetRealNoteRect()
{
	CRect rc;
	GetWindowRect(rc);
	if (m_bMinimized)
	{
		rc.left = rc.right - m_rcRestored.Width();
		rc.bottom = rc.top + m_rcRestored.Height();
	}
	return rc;
}

/**
Returns rollup button rect
*/
/*
CRect CNoteWnd::GetRollupButtonRect()
{
#ifdef RIGHT_ROLLUP_BTN
 	CRect rc(GetCloseButtonRect());
 	rc.left -= s_nCaptionSize;
 	rc.right -= s_nCaptionSize; 
#else
	CClientRect rc(m_hWnd);
	rc.left = s_nCaptionSize + 2;
	rc.right = rc.left + s_nCaptionSize;
#endif
	rc.bottom = s_nCaptionSize;

	return rc;
}
*/
/**
 Returns close button rect
 */
/*
CRect CNoteWnd::GetCloseButtonRect()
{
	CClientRect rc(m_hWnd);
	rc.left = rc.right - s_nCaptionSize;
	rc.bottom = s_nCaptionSize;
	return rc;
}
*/
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
	CClientRect rectDlg(m_hWnd);
	
	// Select the new pen into the device context
	HPEN hOldPen = dc.SelectPen(m_bActive ? m_hPen : m_hGrayPen);
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

	// Clean up
	dc.SelectPen(hOldPen);
}

/**/
void CreateBitmapButton(CBitmapButton& btn, HWND hParent, int nCmdId, int nBmpId, int x, int y, int nTooltipId)
{
	btn.Create(hParent, NULL, NULL, WS_CHILD | WS_VISIBLE, 0, nCmdId);
	btn.SetBitmapButtonExtendedStyle(BMPBTN_HOVER | BMPBTN_AUTOSIZE);
	CImageList il;
	CBitmap bmp;
	bmp.LoadBitmap(nBmpId);
	il.Create(x, y, ILC_COLOR24, 4, 0); //, RGB(0,0,0), IMAGE_BITMAP, 
	il.Add(bmp, RGB(0,0,0));
	btn.SetImageList(il); // il will be deleted in ~CBitmapButtonImpl()
	btn.SetImages(0,1,2,3);
	if (nTooltipId)
	{
		btn.SetToolTipText(RESSTR(nTooltipId));
	}
}

/**
WM_CREATE
*/
LRESULT CNoteWnd::OnCreate(LPCREATESTRUCT lParam)
{
	GetSystemSettings();

	// images for menu icons
	m_ImageList.CreateFromImage(IDB_TRAY_MENU, 16, 1, CLR_DEFAULT, IMAGE_BITMAP, LR_CREATEDIBSECTION | LR_DEFAULTSIZE);

//	m_icon.Create(m_hWnd, GetIconRect(), NULL, WS_CHILD|WS_VISIBLE|SS_ICON|SS_CENTERIMAGE|SS_NOTIFY);
//	m_icon.SetIcon(m_hIconSm);

	AdjustSystemMenu();
/*
	CreateBitmapButton(m_btnClose, m_hWnd, ID_CLOSE, IDB_CLOSE_BTNS, 16, 16, IDS_CLOSE);
	CreateBitmapButton(m_btnRollUp, m_hWnd, ID_ROLLUP, IDB_ROLLUP_BTNS, 16, 16, IDS_ROLLUP);
	CreateBitmapButton(m_btnUnroll, m_hWnd, ID_UNROLL, IDB_UNROLL_BTNS, 16, 16, IDS_UNROLL);
	m_btnUnroll.ShowWindow(SW_HIDE);
*/
	//add caption buttons
	CImageList	il;
//	il.Create(IDB_CLOSE_BTNS_3, 16, 5, RGB(255, 255, 255));
	il.CreateFromImage(IDB_CLOSE_BTNS_3, 16, 1, CLR_DEFAULT, IMAGE_BITMAP, LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	AddButton(ID_CLOSE, 16, 16, il, _T("Close"));

	CImageList	il2;
	il2.CreateFromImage(IDB_ROLLUP_BTNS_3, 16, 1, CLR_DEFAULT, IMAGE_BITMAP, LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	AddButton(ID_ROLLUP, 16, 16, il2, _T("Roll Up"));

	CImageList	il3;
	il3.CreateFromImage(IDB_UNROLL_BTNS_3, 16, 1, CLR_DEFAULT, IMAGE_BITMAP, LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	AddButton(ID_UNROLL, 16, 16, il3, _T("Unroll"), CAPTION_BTN_HIDDEN);

	CImageList	il4;
	il4.CreateFromImage(IDB_NOTES, 16, 1, CLR_DEFAULT, IMAGE_BITMAP, LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	AddButton(ID_SYSMENU, 16, 16, il4, _T("Menu"));

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

	PostMessage(WMU_INITNOTE);

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

/* WMU_INITNOTE */
LRESULT CNoteWnd::OnInitNote(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	m_flagSave = NM_NONE;
	m_edit.SetModify(GetId() == 0);
	m_edit.EmptyUndoBuffer();
	m_bInitialized = TRUE;

	return 0;
}

/**
WM_NCHITTEST
*/
LRESULT CNoteWnd::OnNcHittest(CPoint pt)
{
	ScreenToClient(&pt);
	if (m_bMinimized)
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
	CWindowDC dc(m_hWnd);
//	dc = GetWindowDC();
	ATLASSERT(dc.m_hDC != NULL);

	CWindowRect rcWindow(m_hWnd);
	rcWindow.OffsetRect(-rcWindow.left, -rcWindow.top);

	CClientRect rcClient(m_hWnd);
	rcClient.OffsetRect(GetSystemMetrics(SM_CXSIZEFRAME), 
		GetSystemMetrics(SM_CYSIZEFRAME) + GetSystemMetrics(SM_CYSMCAPTION));

	CRgn rgClient = ::CreateRectRgnIndirect(&rcClient);
	CRgn rgWindow = ::CreateRectRgnIndirect(&rcWindow);
	rgWindow.CombineRgn(rgClient, RGN_DIFF);
	dc.SelectClipRgn(rgWindow);

	{
		CMemoryDC memDc(dc, rcWindow);
		HBRUSH hOldBrush = memDc.SelectBrush(m_hBgBrush);
		HPEN hOldPen = memDc.SelectPen(m_bActive ? m_hPen : m_hGrayPen);
		memDc.Rectangle(&rcWindow);
		memDc.SelectPen(hOldPen);
		memDc.SelectBrush(hOldBrush);
		DrawCaptionButtons(memDc);
		memDc.FillRect(&rcClient, m_hBgBrush);
		if (!m_bMinimized)
		{
			// show label in the caption
			DrawTextInCaption(memDc, GetLabel(), RGB(125, 125, 125));
		}
		else
		{
			DrawTextInCaption(memDc, CApplication::Get().GetNoteCaption(GetText().c_str()), RGB(0, 0, 0));
		}
	}	

//	ReleaseDC(dc);
//	SetMsgHandled(FALSE);
}

/**/
void CNoteWnd::DrawTextInCaption(CDC& dc, const _tstring& text, COLORREF color)
{
	CFontHandle hOldFont = dc.SelectFont(m_hStatusFont);
	dc.SetBkColor(RGB(255, 255, 204));
	dc.SetTextColor(color);

	CRect rc = GetCaptionRect();
	rc.top += 1;
	rc.left += 2;
	dc.DrawText(text.c_str(), -1, rc, DT_LEFT | DT_VCENTER| DT_END_ELLIPSIS);
	dc.SelectFont(hOldFont);
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
		if (!m_bMinimized)
		{
			// show label in the caption
//			DrawTextInCaption(memDc, GetLabel(), RGB(125, 125, 125));

			// draw status bar
			DrawStatusBar(memDc);
		}
		else
		{
//			_tstring s = CApplication::Get().GetNoteCaption(GetText().c_str());
//			DrawTextInCaption(memDc, s, RGB(0, 0, 0));
		}
	}
}

/**
WM_NCACTIVATE
*/
BOOL CNoteWnd::OnNcActivate(BOOL bActive)
{
	m_bActive = bActive;
	SetMsgHandled(FALSE);
	return TRUE;
}

/**
 WM_ACTIVATE
 */
void CNoteWnd::OnActivate(UINT nState, BOOL bMinimized, HWND hWndOther)
{
/*
	m_btnClose.SetImages(nState == 0 ? 3 : 0);
	m_btnClose.Invalidate(FALSE);
   	m_btnClose.UpdateWindow();

	m_btnRollUp.SetImages(nState == 0 ? 3 : 0);
	m_btnRollUp.Invalidate(FALSE);
	m_btnRollUp.UpdateWindow();

	m_btnUnroll.SetImages(nState == 0 ? 3 : 0);
	m_btnUnroll.Invalidate(FALSE);
	m_btnUnroll.UpdateWindow();

	Invalidate(FALSE);
	UpdateWindow();
*/

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
	int nMinHeight = GetMinimizedHeight() + s_nStatusBarSize + 30;
	int nMinWidth = GetMinimizedWidth();
	if (!m_bMinimized)
	{
		lParam->ptMinTrackSize = CPoint(nMinWidth, nMinHeight);
	}
	else
	{
//		lParam->ptMinTrackSize = lParam->ptMaxTrackSize = CPoint(160, GetMinimizedHeight());
	}
}

/**
 WM_SIZE
 */
void CNoteWnd::OnSize(UINT wParam, CSize sz)
{
/*
	m_btnClose.MoveWindow(&GetCloseButtonRect(), TRUE);
	m_btnRollUp.MoveWindow(&GetRollupButtonRect(), TRUE);
	m_btnUnroll.MoveWindow(&GetRollupButtonRect(), TRUE);
*/
	if (!m_bMinimized)
	{
		CClientRect rc(m_hWnd);
		CRect rcCreated(0, rc.bottom - s_nStatusBarSize + 1, 135, rc.bottom);
		m_editCreated.MoveWindow(&rcCreated);

		m_edit.MoveWindow(&(GetClientRect()));
	}
	CWindowRect wr(m_hWnd);
	m_flagSave |= NM_POS;
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
	m_flagSave |= NM_POS;
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
	if (!GetText().empty() && m_bInitialized)
	{
		if (m_edit.GetModify()) // save all if note content has been changed
		{
			m_nNoteId = CApplication::Get().SaveNote(this, NM_ALL);
			m_edit.SetModify(FALSE);
		}
		else if (m_flagSave != NM_NONE)
		{
			m_nNoteId = CApplication::Get().SaveNote(this, m_flagSave);
			m_flagSave = NM_NONE;
		}
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
	_tstring s;
	int len = m_edit.GetWindowTextLength() + 1;
	s.resize(len);
	m_edit.GetWindowText(&s[0],len);
	boost::trim(s);
	if (s[0] == 0)
	{
		s.clear();
	}
	return _tstring(s.c_str());
}

/**/
void CNoteWnd::SetId( int id )
{
	m_nNoteId = id;
}

/**/
void CNoteWnd::SetText(_tstring const& text)
{
		m_edit.SetWindowText(text.c_str());
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
_tstring CNoteWnd::GetLabel() const
{
	return m_label;
}

/**/
void CNoteWnd::SetLabel(LPCTSTR label)
{
	_tstring text = label;
	if (m_label != text)
	{
		m_label = text;
		m_flagSave |= (NM_LABEL | NM_MODIFIED);
		SendMessage(WM_NCPAINT);
//		Invalidate(FALSE);
//		UpdateWindow();
	}
}

/**/
CNoteEdit& CNoteWnd::GetEditor()
{
	return m_edit;
}

/**/
CMenuHandle CNoteWnd::AdjustSystemMenu()
{
	CMenuHandle menu = GetSystemMenu(FALSE);
	if (!menu.IsNull())
	{
		menu.DeleteMenu(SC_MINIMIZE, MF_BYCOMMAND);
		menu.DeleteMenu(SC_MAXIMIZE, MF_BYCOMMAND);
		menu.DeleteMenu(SC_RESTORE, MF_BYCOMMAND);
	}
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

		EscapeFocus();
	}
}

/* ID_RESTORE */
void CNoteWnd::OnRestore(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	CApplication::Get().RestoreNote(GetId());
}

/* ID_DUPLICATE */
void CNoteWnd::OnDuplicate(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	try
	{
		StoreNote(); // save content 
	}
	CATCH_ALL_ERRORS(m_hWnd)
	CApplication::Get().DuplicateNote(GetId());
}


/* ID_DELETE & ID_REMOVE */
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

/* ID_ROLLUP */
void CNoteWnd::OnRollUp(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	Rollup();
}

/* ID_UNROLL */
void CNoteWnd::OnUnroll(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	Unroll();
}

/* ID_SYSMENU */
void CNoteWnd::OnSysMenu(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	POINT p = {0, 0};
	ClientToScreen(&p);
	ShowSystemMenu(p);

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
	StoreNote();
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

	SetMsgHandled(FALSE);
}

/**/
void CNoteWnd::OnNcLButtonDblClk(UINT nHitTest, CPoint point)
{
	ScreenToClient(&point);
	if (nHitTest = HTCAPTION)
	{
		if (m_bMinimized)
		{
			Unroll();
		}
		else
		{
			Rollup();
		}
	}
}

/**/

void CNoteWnd::OnContextMenu(CWindow wnd, CPoint point)
{
	if (m_activeMenu.IsMenu())
	{
		return;
	}
// 	if (GetDeletedDate() != 0 || IsMinimized())
// 	{
		ShowSystemMenu(point);
// 	}
// 	else
// 	{
// 		ShowLabelMenu(point);
// 	}
}

void CNoteWnd::OnMenuRButtonUp(WPARAM wParam, CMenuHandle menu)
{
	POINT pt;
	::GetCursorPos(&pt);

	CMenu mnu;
	mnu.CreatePopupMenu();
	mnu.AppendMenu(MF_STRING, (UINT_PTR)0, _T("test"));
	mnu.TrackPopupMenuEx(TPM_LEFTALIGN | TPM_RECURSE /*| TPM_NOANIMATION*/, pt.x, pt.y, m_hWnd, NULL);
}

int CNoteWnd::OnMouseActivate(CWindow wndTopLevel, UINT nHitTest, UINT message)
{
	SetMsgHandled(FALSE);
	return MA_ACTIVATE;
}

void CNoteWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	SetMsgHandled(FALSE);
}

void CNoteWnd::OnEnterSizeMove()
{
	SetMsgHandled(FALSE);

}

void CNoteWnd::OnExitSizeMove()
{
	if (m_bPrevActive == FALSE && (m_flagSave & NM_POS) && m_bMinimized)
	{
		EscapeFocus();
	}
	SetMsgHandled(FALSE);
}

void CNoteWnd::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	m_bPrevActive = m_bActive;
	SetMsgHandled(FALSE);
}


/**/
void CNoteWnd::Rollup()
{
	if (!m_bMinimized)
	{
		m_editCreated.ShowWindow(SW_HIDE);
		m_edit.ShowWindow(SW_HIDE);

		ShowButton(GetButtonIndex(ID_ROLLUP), false);
		ShowButton(GetButtonIndex(ID_UNROLL), true);
/*
		m_btnRollUp.ShowWindow(SW_HIDE);
		m_btnUnroll.ShowWindow(SW_SHOW);
*/
		GetWindowRect(m_rcRestored);
		CRect rc(m_rcRestored);
		rc.bottom = rc.top + GetMinimizedHeight();
		rc.left = rc.right - GetMinimizedWidth();

		m_bMinimized = TRUE;

		MoveWindow(rc);

		CWindowRect wr(m_hWnd);
//		Invalidate(FALSE);
//		UpdateWindow();

		EscapeFocus();
	}
}

/**/
void CNoteWnd::Unroll()
{
	if (m_bMinimized)
	{
		ShowButton(GetButtonIndex(ID_ROLLUP), true);
		ShowButton(GetButtonIndex(ID_UNROLL), false);

		CRect rc(GetRealNoteRect());
		m_bMinimized = FALSE;
		MoveWindow(rc);

//		Invalidate(FALSE);
//		UpdateWindow();

		m_rcRestored.SetRectEmpty();

		m_editCreated.ShowWindow(SW_SHOW);
		m_edit.ShowWindow(SW_SHOW);

/*
		m_btnRollUp.ShowWindow(SW_SHOW);
		m_btnUnroll.ShowWindow(SW_HIDE);
*/

		m_edit.PostMessage(WM_SETFOCUS);
	}
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


/**/
time_t CNoteWnd::GetDeletedDate() const
{
	return m_dtDeleted;

}

/**/
void CNoteWnd::SetDeletedDate( time_t dt )
{
	m_dtDeleted = dt;
}

/**/
void CNoteWnd::OptionsUpdated()
{
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
}

/**/
BOOL CNoteWnd::IsMinimized()
{
	return m_bMinimized;
}

/**/
void CNoteWnd::OnLabelSelected(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if (nID == LABEL_CMD_FIRST)
	{
		SetLabel(_T(""));
	}
	else if (IS_LABEL_CMD(nID))
	{
		int pos = GET_LABEL_ID_FROM_CMD(nID);
		int i = 1;
		for (std::list<_tstring>::iterator it = m_listLabels.begin();
			it != m_listLabels.end(); ++it)
		{
			if (pos == i)
			{
				if (GetLabel() == _tstring(it->c_str()))
				{
					SetLabel(_T("")); // если выбрана та же метка - очистить
				}
				else
				{
					SetLabel(it->c_str());
				}
				return;
			}
			++i;
		}
	}
}

/**/
void CNoteWnd::OnNewLabel(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	CNewLabelDialog	dlg;
	dlg.m_sLabel = GetLabel();
	if (dlg.DoModal(this->m_hWnd) == IDOK)
	{
		SetLabel(dlg.m_sLabel.c_str());
	}
}

/**/
void CNoteWnd::OnLabelClear(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	SetLabel(_T(""));
}

/**/
void CNoteWnd::SetInitFlags(DWORD nFlags)
{
	m_flagInit = nFlags;
}

/**/
void CNoteWnd::EscapeFocus()
{
	return;

	// set focus to the top window in z-order
	TCHAR name[512];
	HWND next_wnd = ::GetWindow(m_hWnd, GW_HWNDFIRST);
	::GetClassName(next_wnd, name, 512);
	HWND parent_wnd = ::GetParent(next_wnd);
	while (next_wnd && 
		(!::IsWindowVisible(next_wnd) 
		|| parent_wnd == ::GetParent(m_hWnd)
		|| lstrcmp(name, _T("Shell_TrayWnd")) == 0))
	{
		next_wnd = ::GetWindow(next_wnd, GW_HWNDNEXT);
		::GetClassName(next_wnd, name, 512);
		parent_wnd = ::GetParent(next_wnd);
	}
	::SetForegroundWindow(next_wnd);
}

/**/
void CNoteWnd::Refresh()
{
	if (GetDeletedDate() == 0)
	{
//		m_icon.SetIcon(m_hIconSm);
		SetReadOnly(FALSE);
	}
	else
	{
//		m_icon.SetIcon(m_hIconTrash);
		SetReadOnly(TRUE);
	}
}

/**/
void CNoteWnd::SetReadOnly(BOOL bReadOnly /*= TRUE*/)
{
	m_edit.SetReadOnly(bReadOnly);
}

/**/
void CNoteWnd::AssociateImage(CMenuItemInfo& mii, MenuItemData * pMI)
{
	switch (mii.wID)
	{
	case ID_CLIPBRD_COPY:
		pMI->iImage = 4;
		break;
	case ID_DUPLICATE:
		pMI->iImage = 5;
		break;
	case ID_DELETE:
		pMI->iImage = 1;
		break;
	case ID_REMOVE:
		pMI->iImage = 16;
		break;
	case ID_ROLLUP:
		pMI->iImage = 8;
		break;
	case ID_UNROLL:
		pMI->iImage = 9;
		break;
	case ID_EDIT_CUT:
		pMI->iImage = 10;
		break;
	case ID_EDIT_COPY:
		pMI->iImage = 11;
		break;
	case ID_EDIT_PASTE:
		pMI->iImage = 12;
		break;
	case ID_EDIT_REDO:
		pMI->iImage = 13;
		break;
	case ID_EDIT_UNDO:
		pMI->iImage = 14;
		break;
	case ID_EDIT_CLEAR:
		pMI->iImage = 15;
		break;
	default:
		pMI->iImage = -1;
		break;
	}
}

/*
*/
POINT CNoteWnd::GetButtonPos(int index)
{
	CWindowRect rcWindow(m_hWnd);
	rcWindow.OffsetRect(-rcWindow.left, -rcWindow.top);
	rcWindow.DeflateRect(GetSystemMetrics(SM_CXSIZEFRAME), GetSystemMetrics(SM_CYSIZEFRAME));

	int leftAlignIdx = GetButtonIndex(ID_SYSMENU);
	CPoint	pt(rcWindow.left, rcWindow.top);

	if (index >= leftAlignIdx) 
	{
		// left aligment
		pt = CPoint(rcWindow.left, rcWindow.top);

		for (int i = leftAlignIdx + 1; i <= index; i++)
		{
			if (IsButtonVisible(i))
			{
				pt.x += GetButtonSize(i).cx;
				pt.x += CAPTION_BTN_INTERVAL;
			}
		}
	}
	else
	{
		// rigth aligment
		pt = CPoint(rcWindow.right, rcWindow.top);

		for (int i = 0; i <= index; i++)
		{
			if (IsButtonVisible(i))
			{
				pt.x -= GetButtonSize(i).cx;
				pt.x -= CAPTION_BTN_INTERVAL;
			}
		}
	}
	return pt;
}