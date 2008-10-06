/**
 * @file TaskPropsPane.cpp
 *
 * Task properties view window implementation.
 *
 * @author Pavel Mosunov
 * @since 2004-09-02
 */

#include "stdafx.h"
#include "TaskPropsPane.h"

namespace Views
{

/**
 * CTaskPropsPane::PreTranslateMessage
 */
BOOL CTaskPropsPane::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}

/**
 * CTaskPropsPane::OnPaint
 */
LRESULT CTaskPropsPane::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL&	/*bHandled*/)
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


HWND CTaskPropsPane::GetHwnd() const
{
	return m_hWnd;
}

HWND CTaskPropsPane::CreateView(HWND parent)
{
	return Create(parent, rcDefault,_T("Task Props"), WS_CHILD | WS_VISIBLE, WS_EX_CLIENTEDGE );
}

} // namespace Views
