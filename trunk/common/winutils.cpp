#include "stdafx.h"
#include "winutils.h"

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
		CString csName;
		if (::GetClassName(h, csName.GetBufferSetLength(256), 255) 
			&& csName == szClassName)
		{
			return h;
		}
		h = ::GetWindow(h, GW_HWNDNEXT);
	}
	return 0;
}