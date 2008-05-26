/* Pavel Mosunov, 2008-05-26*/

#include "stdafx.h"
#include "menuutils.h"

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
