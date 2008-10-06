/**
 * @file JournalPropsPane.cpp
 *
 * Journal properties view window implementation.
 *
 * @author Pavel Mosunov
 * @since 2004-09-02
 */

#include "stdafx.h"
#include "JournalPropsPane.h"

namespace Views
{

/**
 * 
 */
CJournalPropsPane::~CJournalPropsPane()
{

}


/**
 * CJournalPropsPane::PreTranslateMessage
 */
BOOL CJournalPropsPane::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}

/**
 * CJournalPropsPane::OnPaint
 */
LRESULT CJournalPropsPane::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL&	/*bHandled*/)
{
	CPaintDC dc(m_hWnd);

	CRect rc;
	GetClientRect(&rc);

	CString name;
	GetWindowText(name.GetBuffer(MAX_PATH), MAX_PATH);
	name.ReleaseBuffer();

	dc.DrawText(name, name.GetLength(), rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	return 0;
}

HWND CJournalPropsPane::GetHwnd() const
{
	return m_hWnd;
}

HWND CJournalPropsPane::CreateView(HWND parent)
{
	return Create(parent, rcDefault,_T("Journal Props"), WS_CHILD | WS_VISIBLE, WS_EX_CLIENTEDGE );
}

} // namespace Views
