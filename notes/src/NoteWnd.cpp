/**
 * \file NoteWnd.cpp
 *
 * \since 2007/01/09
 * \author pavel
 */

#include "stdafx.h"
#include "Application.h"
#include "NoteWnd.h"
#include "resource.h"
#include "atlwinmisc.h"
#include "resutils.h"
#include "guiutils.h"
#include "dateutils.h"
#include "Clipboard.h"
#include "NewLabelDialog.h"
#include "noteshook.h"
/*
#include <boost/date_time/posix_time/posix_time.hpp>
#include "boost/date_time/c_local_time_adjustor.hpp"
*/
#include <sys/types.h>
#include <sys/timeb.h>

#include "dateutils.h"

using namespace dateutils;


const INT s_nCornerSize = 14;
const INT s_nStatusBarSize = 15;
const INT s_nCaptionDelta = 4;

const COLORREF black = RGB(0,0,0);
const COLORREF gray = RGB(128, 128, 128);
const COLORREF yellow = RGB(255, 255, 204);

CBrush CNoteWnd::m_hBgBrush = CreateSolidBrush(RGB(255, 255, 204));
CIcon CNoteWnd::m_hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME),
								  IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);

CPen CNoteWnd::m_hPen = ::CreatePen(PS_SOLID, 1, RGB(0,0,0));
CPen CNoteWnd::m_hGrayPen = ::CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
CFont CNoteWnd::m_hStatusFont = CFontHandle().CreatePointFont(80, _T("MS Shell Dlg"));

/**
 Constructor
 */
CNoteWnd::CNoteWnd(int nNoteId /*= 0*/) 
:	m_nNoteId(nNoteId),
	m_bMinimized(FALSE),
	m_bInitialized(FALSE),
	m_bActive(FALSE),
	m_bActiveApp(TRUE),
	m_flagSave(NM_NONE),
	m_flagInit(NF_NONE),
	m_bCaptionClick(FALSE)
{
	timebn::clear(m_dtCreated);
	timebn::clear(m_dtDeleted);



	 struct _timeb timebuffer;
	 struct _timeb timebuffer2;
	 struct _timeb timebuffer3;
	 struct _timeb timebuffer4;
	 struct _timeb timebuffer5;

	 time_t tt;
	 time(&tt);
	_ftime_s(&timebuffer);
	Sleep(1);
	_ftime_s(&timebuffer2);
	Sleep(1);
	_ftime_s(&timebuffer3);
	Sleep(1);
	_ftime_s(&timebuffer4);
	Sleep(1);
	_ftime_s(&timebuffer5);

	_tstring s4 = ToString(timebuffer.time, TRUE);
	_tstring s5 = ToString(timebuffer.time, FALSE);


    long timezone;
    _get_timezone( &timezone );
/*
	
	using namespace boost::posix_time;
//	using namespace boost::date_time;

	ptime t(microsec_clock::universal_time());

	ptime lt = boost::date_time::c_local_adjustor<ptime>::utc_to_local(t);
	
	string s = to_iso_string(t);
	string s2 = to_iso_string(t);
	string ls = to_iso_string(lt);
	ptime lt2 = from_iso_string(ls);

	wtime_facet* ltf = new wtime_facet();

	wstringstream ss;
	ss.imbue(locale(locale::classic(), ltf));
//	ss.imbue(locale(ss.getloc(), ltf));

	ltf->format(L"%a %b %d, %H:%M %z");

	ss << t;

	wstring s3 = ss.str();
	*/
/*
	  local_time_facet* output_facet = new local_time_facet();
  local_time_input_facet* input_facet = new local_time_input_facet();
  ss.imbue(locale(locale::classic(), output_facet));
  ss.imbue(locale(ss.getloc(), input_facet));
  */
}

/**
 Destructor
 */
CNoteWnd::~CNoteWnd()
{
}

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
	if (IsDeleted())
	{
		menuPopup = m_activeMenu.GetSubMenu(1);
		if (menuPopup.m_hMenu == NULL)
		{
			return;
		}
	}
	else
	{
		menuPopup = m_activeMenu.GetSubMenu(0);
		if (menuPopup.m_hMenu == NULL)
		{
			return;
		}
		CMenuHandle menuLabels = menuPopup.GetSubMenu(1);
		PopulateLabelMenu(menuLabels);
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
 Returns caption coordinates between caption buttons
 */
CRect CNoteWnd::GetInnerCaptionRect()
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
}

/*
Return full caption coordinates
*/
CRect CNoteWnd::GetOutterCaptionRect()
{
	CWindowRect rc(m_hWnd);
	if (!m_bMinimized)
	{
		rc.DeflateRect(GetSystemMetrics(SM_CYSIZEFRAME), GetSystemMetrics(SM_CYSIZEFRAME));
		rc.bottom = rc.top + GetSystemMetrics(SM_CYSMCAPTION) + s_nCaptionDelta;
	}

	return rc;
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
	return CApplication::Get().GetOptions().GetNewNoteSize().cx;
}

/**/
CRect CNoteWnd::GetClientRect()
{
	CClientRect rc(m_hWnd);
	rc.top += s_nCaptionDelta;
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
 Returns bottom border for window sizing
 */
CRect CNoteWnd::GetBottomBorderRect()
{
	CClientRect rc(m_hWnd);
	rc.top = rc.bottom - s_nCornerSize;
	return rc;
}

/**
 Returns bottom right corner for window sizing
 */
CRect CNoteWnd::GetBottomRightRect()
{
	CRect rc = GetBottomBorderRect();
	rc.left = rc.right - s_nCornerSize;
	return rc;
}

/**
 Last message - destroy object
 */
void CNoteWnd::OnFinalMessage(HWND hWnd)
{
	CApplication::Get().OnNoteClosed(this);
	EscapeFocus();
	
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


	// draw text
	if (!timebn::isempty(m_dtCreated))
	{
		_tstring sDate = ToString(m_dtCreated.time, _T("%#d %b %Y, %H:%M"));

		CFontHandle hOldFont = dc.SelectFont(m_hStatusFont);
		dc.SetBkColor(RGB(255, 255, 204));
		dc.SetTextColor(RGB(125, 125, 125));

		CRect rc = GetBottomBorderRect();
		rc.top += 1;
		rc.left += 2;
		rc.right -= s_nCornerSize - 2;
		dc.DrawText(RESSTR_FMT(IDS_CREATED_FRM, sDate.c_str()).c_str(), -1, rc, DT_LEFT | DT_VCENTER| DT_END_ELLIPSIS);
		dc.SelectFont(hOldFont);
	}
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

	AdjustSystemMenu();
	//add caption buttons
	CImageList	il;
	il.CreateFromImage(IDB_CLOSE_BTNS_3, 24, 1, CLR_DEFAULT, IMAGE_BITMAP, LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	AddButton(ID_CLOSE, 24, 16, il, /*_T("Close")*/NULL);

	CImageList	il2;
	il2.CreateFromImage(IDB_ROLLUP_BTNS_3, 24, 1, CLR_DEFAULT, IMAGE_BITMAP, LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	AddButton(ID_ROLLUP, 24, 16, il2, _T("Roll Up"));
/*
	CImageList	il3;
	il3.CreateFromImage(IDB_UNROLL_BTNS_3, 16, 1, CLR_DEFAULT, IMAGE_BITMAP, LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	AddButton(ID_UNROLL, 16, 16, il3, _T("Unroll"), CAPTION_BTN_HIDDEN);
*/
	CImageList	il4;
	il4.CreateFromImage(IDB_NOTES, 16, 1, CLR_DEFAULT, IMAGE_BITMAP, LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	AddButton(ID_SYSMENU, 16, 16, il4, _T("Menu"));

	CImageList	il5;
	il5.CreateFromImage(IDB_TRASH_BTNS, 16, 1, CLR_DEFAULT, IMAGE_BITMAP, LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	AddButton(ID_TRASHSYSMENU, 16, 16, il5, _T("Menu"), CAPTION_BTN_HIDDEN);

	m_edit.Create(m_hWnd);
	m_edit.Init(yellow);
	static_cast<CWindow>(m_edit).SetFocus();
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
	m_edit.EmptyUndoBuffer();
	m_bInitialized = TRUE;
	if (!GetText().empty())
	{
//		PostMessage(WMU_ESCAPEFOCUS);
	}
	return 0;
}

/**
WM_NCHITTEST
*/
LRESULT CNoteWnd::OnNcHittest(CPoint pt)
{	
	if (ID_SYSMENU == GetButtonId(GetButtonAtScreenPos(pt)))
	{
		return HTSYSMENU;
	}
	if (ID_CLOSE == GetButtonId(GetButtonAtScreenPos(pt)))
	{
		return HTCLOSE;
	}
	if (GetOutterCaptionRect().PtInRect(pt))
	{
		return HTCAPTION;
	}

	CPoint client_pt(pt);
	ScreenToClient(&client_pt);

	CRect rc = GetBottomBorderRect();
	if (rc.PtInRect(client_pt))
	{
		if (client_pt.x >= rc.right - s_nCornerSize)
		{
			return HTBOTTOMRIGHT;
		}
		else if (client_pt.x <= s_nCornerSize)
		{
			return HTBOTTOMLEFT;
		}
		else
		{
			return HTBOTTOM;
		}
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
}

/**/
void CNoteWnd::DrawTextInCaption(CDC& dc, const _tstring& text, COLORREF color)
{
	CFontHandle hOldFont = dc.SelectFont(m_hStatusFont);
	dc.SetBkColor(RGB(255, 255, 204));
	dc.SetTextColor(color);

	CRect rc = GetInnerCaptionRect();
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
			// draw status bar
			DrawStatusBar(memDc);
		}
	}
}

/**
WM_NCACTIVATE
*/
BOOL CNoteWnd::OnNcActivate(BOOL bActive)
{
	m_bActive = bActive;
	SendMessage(WM_NCPAINT);
	SetMsgHandled(FALSE);
	return TRUE;
}

/**
 WM_ACTIVATE
 */
void CNoteWnd::OnActivate(UINT nState, BOOL bMinimized, HWND hWndOther)
{

	//SendMessage(WM_NCPAINT);

	if (nState == WA_INACTIVE)
	{
		StoreNote();
	}
	PostMessage(WMU_ACTIVATEPOST);
}

/* WM_MOUSEACTIVATE */
int CNoteWnd::OnMouseActivate(CWindow wndTopLevel, UINT nHitTest, UINT message)
{

	SetMsgHandled(FALSE);
	return MA_ACTIVATE;
}

/* WM_ACTIVATEAPP */
void CNoteWnd::OnActivateApp(BOOL bActive, DWORD dwThreadID)
{
	m_bActiveApp = bActive;
	SetMsgHandled(FALSE);
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
}

/* WM_NCRBUTTONUP */
void CNoteWnd::OnNcRButtonUp(UINT nHitTest, CPoint point)
{
	ShowSystemMenu(point);
}

/* WM_LBUTTONUP */
void CNoteWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	SetMsgHandled(FALSE);
}

/* WM_ENTERSIZEMOVE */
void CNoteWnd::OnEnterSizeMove()
{
	m_bCaptionClick = FALSE; // начали двигать, сбросить флаг клика на заголовке
	SetMsgHandled(FALSE);
}

/**
 WM_SIZE
 */
void CNoteWnd::OnSize(UINT wParam, CSize sz)
{
	if (!m_bMinimized)
	{
		CClientRect rc(m_hWnd);
		m_edit.MoveWindow(&(GetClientRect()));
	}

	m_flagSave |= NM_POS;
}

/*
WM_MOVE
*/
void CNoteWnd::OnMove(CPoint pt)
{
	m_flagSave |= NM_POS;
}

/* WM_EXITSIZEMOVE */
void CNoteWnd::OnExitSizeMove()
{
	SetMsgHandled(FALSE);
/*
	if (!GetText().empty() && !m_edit.GetModify())
	{
		PostMessage(WMU_ESCAPEFOCUS);
	}
	*/
}

/* WM_NCLBUTTONDOWN */
void CNoteWnd::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	SetMsgHandled(FALSE);
}

/* WM_NCLBUTTONDOWN */
void CNoteWnd::OnNcLButtonDownDef(UINT nHitTest, CPoint point)
{
	SetMsgHandled(FALSE);
//	return;
	if (nHitTest == HTCAPTION)
	{
		m_bCaptionClick = TRUE;
		BOOL bAlreadyActive = m_bActive;
//		BOOL bAlreadyActiveApp = m_bActiveApp;

		SetMsgHandled(TRUE);
		DefWindowProc(); // обработка по умолчанию, здесь придут OnEnterSizeMove, OnExitSizeMove и другие сообщения

		// если окно начнут двигать m_bCaptionClick станет FALSE в OnEnterSizeMove()
		if (m_bCaptionClick == TRUE) // слик на минимизированном окне
		{
			if (m_bMinimized)
			{
				Unroll();
//				if (!bAlreadyActive/*App*/)
//				{
//					PostMessage(WMU_ESCAPEFOCUS);
//				}
			}
		}
		else 
		{
//			if (!bAlreadyActive/*App /*|| m_bMinimized*/) // перемещение окна за заголовок
//			if (!GetText().empty() && !m_edit.GetModify())
			if (m_bMinimized)
			{
				PostMessage(WMU_ESCAPEFOCUS);
			}
		}
	}
}

/*WM_NCLBUTTONUP*/
void CNoteWnd::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	// приходит только после dblclk
	SetMsgHandled(FALSE);
}

/*
WM_SETFOCUS
*/
void CNoteWnd::OnFocus(HWND hWnd)
{
	static_cast<CWindow>(m_edit).SetFocus();
	SetMsgHandled(FALSE);
}



/*
WM_CTLCOLORSTATIC
*/
HBRUSH CNoteWnd::OnCtlColorStatic(CDCHandle dc, CStatic wndStatic)
{
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
			m_edit.SetSavePoint();
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
_tstring CNoteWnd::GetText()
{
	return m_edit.GetText();
}

/**/
void CNoteWnd::SetId( int id )
{
	m_nNoteId = id;
}

/**/
void CNoteWnd::SetText(_tstring const& text)
{
	m_edit.SetText(text);
}

/**/
_timeb CNoteWnd::GetCreatedDate() const
{
	return m_dtCreated;
}

/**/
void CNoteWnd::SetCreatedDate(_timeb dt)
{
	m_dtCreated = dt;
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
	}
}

/**/
/*
CNoteEdit& CNoteWnd::GetEditor()
{
	return m_edit;
}
*/

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

//		EscapeFocus();
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
	if (GetKeyState(VK_CONTROL) & 0x8000)
	{
		CApplication::Get().EnumNoteWnds(&CNoteWnd::Rollup); // close all but this
	}
	else
	{
		Rollup();
	}
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
/* тут обработку клика на иконке
	if (GetButtonAtPos(point) == GetButtonIndex(ID_SYSMENU))
	{
		PostMessage(WM_CLOSE);
		return;
	}
*/
//	ScreenToClient(&point);
	
	if (nHitTest == HTCAPTION)
	{
		if (m_bMinimized)
		{
			//Unroll();
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
	if (wnd == m_edit.m_hWnd)
	{
		CMenu hEditMenu;
		hEditMenu.LoadMenu(IDR_EDIT);
		CMenuHandle menuPopup = hEditMenu.GetSubMenu(0);
		menuPopup.TrackPopupMenu(TPM_LEFTALIGN|TPM_TOPALIGN|TPM_LEFTBUTTON,
			point.x, point.y, m_hWnd);
	}
	else
	{
		if (IsMinimized())
		{
			ShowLabelMenu(point);
		}
		else
		{
			ShowSystemMenu(point);
		}
	}
}


/**/
void CNoteWnd::Rollup()
{
	if (!m_bMinimized)
	{
		m_edit.ShowWindow(SW_HIDE);

		ShowButton(GetButtonIndex(ID_ROLLUP), false);
/*	
		ShowButton(GetButtonIndex(ID_UNROLL), true);
*/	
		GetWindowRect(m_rcRestored);
		CRect rc(m_rcRestored);
		rc.bottom = rc.top + GetMinimizedHeight();
		rc.left = rc.right - GetMinimizedWidth();

		m_bMinimized = TRUE;

		MoveWindow(rc);

		PostMessage(WMU_ESCAPEFOCUS);
	}
}

/**/
void CNoteWnd::Unroll()
{
	if (m_bMinimized)
	{
		ShowButton(GetButtonIndex(ID_ROLLUP), true);
/*
		ShowButton(GetButtonIndex(ID_UNROLL), false);
*/
		CRect rc(GetRealNoteRect());
		m_bMinimized = FALSE;
		MoveWindow(rc);

		m_rcRestored.SetRectEmpty();

		m_edit.ShowWindow(SW_SHOW);

//		m_edit.PostMessage(WM_SETFOCUS);
		if (!GetText().empty())
		{
//			PostMessage(WMU_ESCAPEFOCUS);
		}
	}
}


/**/
LRESULT CNoteWnd::OnLink(LPNMHDR pnmh)
{
#ifdef RICHEDIT	
	ENLINK* pLinkInfo = (ENLINK*)pnmh;
	if (pLinkInfo->msg == WM_LBUTTONUP)
	{
		_tstring txt;
		txt.resize(pLinkInfo->chrg.cpMax - pLinkInfo->chrg.cpMin + 1);
		m_edit.GetTextRange(pLinkInfo->chrg.cpMin, pLinkInfo->chrg.cpMax, &txt[0]);
		ShellExecute(NULL, NULL, txt.c_str(), NULL, NULL, SW_SHOW);
	}
#endif
	return 0;
}


/**/
_timeb CNoteWnd::GetDeletedDate() const
{
	return m_dtDeleted;

}

/**/
void CNoteWnd::SetDeletedDate( _timeb dt )
{
	m_dtDeleted = dt;
}

/**/
void CNoteWnd::OptionsUpdated()
{
	COptions::FontSize fs = CApplication::Get().GetOptions().GetFontSize();
	m_edit.SetFontSize(fs == COptions::FS_SMALL ? 160 : (fs == COptions::FS_MEDIUM ? 200 : 240));
}

/**/
BOOL CNoteWnd::IsMinimized()
{
	return m_bMinimized;
}

/**/
BOOL CNoteWnd::IsDeleted()
{
	return !timebn::isempty(m_dtDeleted);
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

/*WMU_ESCAPEFOCUS*/
LRESULT CNoteWnd::OnEscapeFocus(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	EscapeFocus();
	return 0;
}

/* WMU_ACTIVATEPOST */
LRESULT CNoteWnd::OnActivatePost(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

/**/
void CNoteWnd::EscapeFocus()
{
	HWND prevwnd = GetPrevActiveWindow();
	if (prevwnd)
	{
		::SetForegroundWindow(prevwnd);
	}

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
	if (IsDeleted())
	{
		ShowButton(GetButtonIndex(ID_SYSMENU), false);
		ShowButton(GetButtonIndex(ID_TRASHSYSMENU), true);

		SetReadOnly(TRUE);
	}
	else
	{
		ShowButton(GetButtonIndex(ID_SYSMENU), true);
		ShowButton(GetButtonIndex(ID_TRASHSYSMENU), false);

		SetReadOnly(FALSE);
	}
	SendMessage(WM_NCPAINT);
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
		pMI->iImage = 17;//8;
		break;
	case ID_UNROLL:
		pMI->iImage = 18;//9;
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
		int right = rcWindow.left;
		for (int i = leftAlignIdx; i <= index; i++)
		{
			if (IsButtonVisible(i))
			{
				pt.x = right;
				right = pt.x + GetButtonSize(i).cx;
				right += CAPTION_BTN_INTERVAL;
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