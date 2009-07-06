#pragma once

namespace winutils
{
	BOOL WinPosIsValid(const CRect& WinPos);
	HWND GetTopWnd(LPCTSTR szClassName);
	void AdjustScreenRect(CRect& rc);
	HWND GetPopupParent(HWND hwnd);
}