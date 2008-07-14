/* Pavel Mosunov, 2008-05-26*/

namespace menuutils
{
	BOOL SetMenuItemEnable(HMENU hMenu, WORD wId, BOOL bEnable);
	_tstring GetMenuString(HMENU hMenu, UINT nIDItem, UINT nFlags);
}