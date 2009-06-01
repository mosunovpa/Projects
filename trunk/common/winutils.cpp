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
