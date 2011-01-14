/* Pavel Mosunov, 2008-05-26*/

#include "stdafx.h"
#include "menuutils.h"

/**/
BOOL menuutils::SetMenuItemEnable(HMENU hMenu, WORD wId, BOOL bEnable)
{
	MENUITEMINFO mif;
	ZeroMemory(&mif, sizeof(MENUITEMINFO));
	mif.cbSize = sizeof(MENUITEMINFO);
	mif.fMask = MIIM_STATE;
	mif.wID = wId;
	mif.fState = bEnable ? MFS_ENABLED : MFS_DISABLED;
	return ::SetMenuItemInfo(hMenu, wId, FALSE, &mif);
}

/**/
_tstring menuutils::GetMenuString( HMENU hMenu, UINT nIDItem, UINT nFlags )
{
	int len = ::GetMenuString(hMenu, nIDItem, 0, 0, nFlags);
	_tstring s(len + 1, 0);
	::GetMenuString(hMenu, nIDItem, &s[0], s.size(), nFlags);
	return s;
}

/**/
HWND menuutils::FindMenuWindow(HMENU hMenu)
{
	HWND hCurWnd = NULL;

	do 
	{
		hCurWnd = ::FindWindowEx(NULL, hCurWnd, _T("#32768"), NULL);
		if(hCurWnd)
		{
			HMENU hCurMenu = (HMENU)::SendMessage(hCurWnd, MN_GETHMENU, 0, 0);
			if(hCurMenu == hMenu)
				return hCurWnd;
		}
	} 
	while(hCurWnd);

	return NULL;
}

/**/
BOOL menuutils::UpdateMenuWindow(CMenuHandle const& menu)
{
	CWindow wndMenu = menuutils::FindMenuWindow(menu);
	if (::IsWindowVisible(wndMenu))
	{
		wndMenu.Invalidate(FALSE);
		wndMenu.UpdateWindow();
		return TRUE;
	}
	return FALSE;
}
