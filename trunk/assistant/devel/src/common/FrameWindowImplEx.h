/**
 * \file FrameWindowImplEx.h
 * 
 *
 * \author Pavel Mosunov
 * \since 2005/12/28
 */

#ifndef FrameWindowImplEx_h__
#define FrameWindowImplEx_h__

#include <atlframe.h>

#define ATL_SIMPLE_VERT_TOOLBAR_PANE_STYLE \
	(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS \
	| TBSTYLE_TOOLTIPS | TBSTYLE_FLAT | TBSTYLE_WRAPABLE \
	| CCS_NORESIZE | CCS_NOPARENTALIGN | CCS_VERT \
	)

#define ATL_SIMPLE_LEFT_TOOLBAR_PANE_STYLE  \
	ATL_SIMPLE_VERT_TOOLBAR_PANE_STYLE | CCS_LEFT

#define ATL_SIMPLE_VERT_REBAR_STYLE \
	(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS \
	| RBS_VARHEIGHT | RBS_AUTOSIZE \
	| CCS_NOPARENTALIGN | CCS_VERT \
	)

#define ATL_SIMPLE_LEFT_REBAR_STYLE \
	ATL_SIMPLE_VERT_REBAR_STYLE | CCS_LEFT

template <class T, class TBase = ATL::CWindow, class TWinTraits = ATL::CFrameWinTraits>
class CFrameWindowImplEx : public CFrameWindowImpl< T, TBase, TWinTraits >
{
public:
	HWND m_hWndLeftToolBar;

	void UpdateLayout(BOOL bResizeBars = TRUE)
	{
		RECT rect = { 0 };
		GetClientRect(&rect);

		// position bars and offset their dimensions
		UpdateBarsPosition(rect, bResizeBars);

		// resize left toolbar
		if(m_hWndLeftToolBar != NULL && ((DWORD)::GetWindowLong(m_hWndLeftToolBar, GWL_STYLE) & WS_VISIBLE))
		{
			RECT rectTB = { 0 };
			::GetWindowRect(m_hWndLeftToolBar, &rectTB);
			rect.left += rectTB.right - rectTB.left;
			::MoveWindow(m_hWndLeftToolBar, 0, rect.top, rect.left, rect.bottom - rect.top, TRUE);
			::InvalidateRect(m_hWndLeftToolBar, NULL, TRUE);
		}

		// resize client window
		if(m_hWndClient != NULL)
		{
			::SetWindowPos(m_hWndClient, NULL, rect.left, rect.top,
				rect.right - rect.left, rect.bottom - rect.top,
				SWP_NOZORDER | SWP_NOACTIVATE);
		}
	}

	BOOL CreateSimpleLeftToolBar(UINT nResourceID = 0, DWORD dwStyle = ATL_SIMPLE_LEFT_TOOLBAR_PANE_STYLE, UINT nID = ATL_IDW_TOOLBAR)
	{
		if(nResourceID == 0)
			nResourceID = T::GetWndClassInfo().m_uCommonResourceID;
		ATLASSERT(!::IsWindow(m_hWndLeftToolBar));

		m_hWndLeftToolBar = CreateSimpleReBarCtrl(m_hWnd, ATL_SIMPLE_LEFT_REBAR_STYLE, ATL_IDW_TOOLBAR);

		HWND hWndLeftToolBar = T::CreateSimpleToolBarCtrl(m_hWnd, nResourceID, FALSE, dwStyle, nID);
		AddSimpleReBarBandCtrl(m_hWndLeftToolBar, hWndLeftToolBar, 0);

		return (m_hWndLeftToolBar != NULL);
	}

	// Implementation - chevron menu support
#if (_WIN32_IE >= 0x0500) && !defined(_WIN32_WCE)
	bool PrepareChevronMenu(_ChevronMenuInfo& cmi)
	{
		// get rebar and toolbar
		REBARBANDINFO rbbi = { 0 };
		rbbi.cbSize = sizeof(REBARBANDINFO);
		rbbi.fMask = RBBIM_CHILD;
		BOOL bRet = (BOOL)::SendMessage(cmi.lpnm->hdr.hwndFrom, RB_GETBANDINFO, cmi.lpnm->uBand, (LPARAM)&rbbi);
		ATLASSERT(bRet);

		// assume the band is a toolbar
		ATL::CWindow wnd = rbbi.hwndChild;
		int nCount = (int)wnd.SendMessage(TB_BUTTONCOUNT);
		if(nCount <= 0)   // probably not a toolbar
			return false;

		// check if it's a command bar
		CMenuHandle menuCmdBar = (HMENU)wnd.SendMessage(CBRM_GETMENU);
		cmi.bCmdBar = (menuCmdBar.m_hMenu != NULL);

		// build a menu from hidden items
		CMenuHandle menu;
		bRet = menu.CreatePopupMenu();
		ATLASSERT(bRet);
		RECT rcClient = { 0 };
		bRet = wnd.GetClientRect(&rcClient);
		ATLASSERT(bRet);
		for(int i = 0; i < nCount; i++)
		{
			TBBUTTON tbb = { 0 };
			bRet = (BOOL)wnd.SendMessage(TB_GETBUTTON, i, (LPARAM)&tbb);
			ATLASSERT(bRet);
			// skip hidden buttons
			if((tbb.fsState & TBSTATE_HIDDEN) != 0)
				continue;
			RECT rcButton = { 0 };
			bRet = (BOOL)wnd.SendMessage(TB_GETITEMRECT, i, (LPARAM)&rcButton);
			ATLASSERT(bRet);
			bool bEnabled = ((tbb.fsState & TBSTATE_ENABLED) != 0);

			// check if visible
			DWORD dwStyle = (DWORD)wnd.SendMessage(TB_GETSTYLE, 0, 0L);
			BOOL bVisible = dwStyle & CCS_VERT ? 
				rcButton.bottom > rcClient.bottom : rcButton.right > rcClient.right;
			if(bVisible)
			{
				if(tbb.fsStyle & BTNS_SEP)
				{
					if(menu.GetMenuItemCount() > 0)
						menu.AppendMenu(MF_SEPARATOR);
				}
				else if(cmi.bCmdBar)
				{
					const int cchBuff = 200;
					TCHAR szBuff[cchBuff] = { 0 };
					CMenuItemInfo mii;
					mii.fMask = MIIM_TYPE | MIIM_SUBMENU;
					mii.dwTypeData = szBuff;
					mii.cch = cchBuff;
					bRet = menuCmdBar.GetMenuItemInfo(i, TRUE, &mii);
					ATLASSERT(bRet);
					// Note: CmdBar currently supports only drop-down items
					ATLASSERT(::IsMenu(mii.hSubMenu));
					bRet = menu.AppendMenu(MF_STRING | MF_POPUP | (bEnabled ? MF_ENABLED : MF_GRAYED), (UINT_PTR)mii.hSubMenu, mii.dwTypeData);
					ATLASSERT(bRet);
				}
				else
				{
					// get button's text
					const int cchBuff = 200;
					TCHAR szBuff[cchBuff] = { 0 };
					LPTSTR lpstrText = szBuff;
					TBBUTTONINFO tbbi = { 0 };
					tbbi.cbSize = sizeof(TBBUTTONINFO);
					tbbi.dwMask = TBIF_TEXT;
					tbbi.pszText = szBuff;
					tbbi.cchText = cchBuff;
					if(wnd.SendMessage(TB_GETBUTTONINFO, tbb.idCommand, (LPARAM)&tbbi) == -1 || lstrlen(szBuff) == 0)
					{
						// no text for this button, try a resource string
						lpstrText = _T("");
#if (_ATL_VER >= 0x0700)
						int nRet = ::LoadString(ATL::_AtlBaseModule.GetResourceInstance(), tbb.idCommand, szBuff, cchBuff);
#else // !(_ATL_VER >= 0x0700)
						int nRet = ::LoadString(_Module.GetResourceInstance(), tbb.idCommand, szBuff, cchBuff);
#endif // !(_ATL_VER >= 0x0700)
						for(int n = 0; n < nRet; n++)
						{
							if(szBuff[n] == _T('\n'))
							{
								lpstrText = &szBuff[n + 1];
								break;
							}
						}
					}
					bRet = menu.AppendMenu(MF_STRING | (bEnabled ? MF_ENABLED : MF_GRAYED), tbb.idCommand, lpstrText);
					ATLASSERT(bRet);
				}
			}
		}

		if(menu.GetMenuItemCount() == 0)   // no hidden buttons after all
		{
			menu.DestroyMenu();
			::MessageBeep((UINT)-1);
			return false;
		}

		cmi.hMenu = menu;
		return true;
	}
};
#endif // (_WIN32_IE >= 0x0500) && !defined(_WIN32_WCE)

#endif // FrameWindowImplEx_h__
