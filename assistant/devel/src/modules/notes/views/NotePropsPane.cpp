/**
 * @file NotePropsPane.cpp
 *
 * Note properties view window implementation.
 *
 * @author Pavel Mosunov
 * @since 2004-09-02
 */

#include "stdafx.h"
#include "NotePropsPane.h"

namespace Views
{

/**
 * CNotePropsPane::PreTranslateMessage
 */
BOOL CNotePropsPane::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}

/**
 * CNotePropsPane::OnPaint
 */
void CNotePropsPane::OnPaint(HDC hdc)
{
	CPaintDC dc(m_hWnd);

	CRect rc;
	GetClientRect(&rc);

	CString name;
	GetWindowText(name.GetBuffer(MAX_PATH), MAX_PATH);
	name.ReleaseBuffer();

	dc.DrawText(name, name.GetLength(), rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

HWND CNotePropsPane::GetHwnd() const
{
	return m_hWnd;
}

HWND CNotePropsPane::CreateView(HWND parent)
{
	return Create(parent, rcDefault,_T("Notes Props"), WS_CHILD | WS_VISIBLE, WS_EX_CLIENTEDGE );
}

} // namespace Views
