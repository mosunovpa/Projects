#include "stdafx.h"
#include "winutils.h"
#include "atlwinmisc.h"

BOOL winutils::WinPosIsValid(const CRect& WinPos)
{
	CRect rc;
	::GetWindowRect(GetDesktopWindow(), &rc);
	return rc.PtInRect(WinPos.TopLeft()) && rc.PtInRect(WinPos.BottomRight());
}

HWND winutils::GetTopWnd( LPCTSTR szClassName )
{
	HWND h = ::GetWindow(GetDesktopWindow(), GW_CHILD);
	while (h)
	{
		_tstring sName;
		sName.resize(255);
		if (::GetClassName(h, &sName[0], 255) 
			&& lstrcmp(sName.c_str(), szClassName) == 0)
		{
			return h;
		}
		h = ::GetWindow(h, GW_HWNDNEXT);
	}
	return 0;
}

void winutils::AdjustScreenRect(CRect& rc)
{
	CWindowRect rcDesktop(::GetDesktopWindow());
	CWindowRect rcTaskBar(::FindWindow(_T("Shell_TrayWnd"), NULL));
	CRect rcDesk;
	rcDesk.SubtractRect(rcDesktop, rcTaskBar);

	if (rc.right > rcDesk.right)
	{
		rc.MoveToX(rcDesk.right - rc.Width());
	}
	if (rc.bottom > rcDesk.bottom)
	{
		rc.MoveToY(rcDesk.bottom - rc.Height());
	}
	if (rc.left < rcDesk.left)
	{
		rc.MoveToX(rcDesk.left);
	}
	if (rc.top < rcDesk.top)
	{
		rc.MoveToY(rcDesk.top);
	}
}

HWND winutils::GetPopupParent(HWND hwnd)
{
	HWND hParent = GetParent(hwnd);
	while (hParent)
	{
		LONG lStyle = ::GetWindowLong(hParent, GWL_STYLE);
		if ((lStyle & WS_POPUP) == WS_POPUP)
		{
			return hParent;
		}
		hParent = GetParent(hParent);
	}
	return 0;
}
