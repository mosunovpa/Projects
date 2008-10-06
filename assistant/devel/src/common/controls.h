/*
file controls.h
Created 2007-12-28 by Pavel Mosunov
*/

#pragma once

/* no blinked list view control */

class CListViewCtrlEx : public CSortListViewCtrlImpl<CListViewCtrlEx>
{
public:
	DECLARE_WND_SUPERCLASS(_T("ListViewCtrlEx"), GetWndClassName())

	BEGIN_MSG_MAP(CListViewCtrlEx)
		MSG_WM_PAINT(OnPaint)
		MSG_WM_ERASEBKGND(OnErasebkgnd)
		CHAIN_MSG_MAP(CSortListViewCtrlImpl<CListViewCtrlEx>)
	END_MSG_MAP()

	void OnPaint(HDC hdc)
	{
		CPaintDC dcPaint(m_hWnd);
		CRect rcClient;
		GetClientRect(&rcClient);
		CMemoryDC dcMem(dcPaint, rcClient);
		dcMem.FillRect(&rcClient, GetSysColorBrush(COLOR_WINDOW));
		DefWindowProc(WM_PAINT, (WPARAM)dcMem.m_hDC, 0);
	}

	LRESULT OnErasebkgnd(HDC hdc)
	{
		return 0;
	}
};