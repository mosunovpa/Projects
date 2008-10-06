/**
 * @file JournalListPane.cpp
 *
 * Journal list view window implementation.
 *
 * @author Pavel Mosunov
 * @since 2004-09-02
 */

#include "stdafx.h"
#include "JournalListPane.h"

namespace Views
{
/**
 * 
 */
CJournalListPane::~CJournalListPane()
{

}

/**
 * CJournalListPane::PreTranslateMessage
 */
BOOL CJournalListPane::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}

/**
 * CJournalListPane::OnPaint
 */
LRESULT CJournalListPane::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL&	/*bHandled*/)
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

HWND CJournalListPane::GetHwnd() const
{
	return m_hWnd;
}

HWND CJournalListPane::CreateView(HWND parent)
{
	return Create(parent, rcDefault,_T("Journal List"), WS_CHILD | WS_VISIBLE, WS_EX_CLIENTEDGE );
}

} // namespace Views
