#pragma once

#include "atlwinmisc.h"
#include "winutils.h"

#define MENU_TOOLTIP_TIMER_ID 654


/**/
template <class T>
class CMenuTooltip
{
public:
	/**/
	CMenuTooltip() : 
		m_nTimer(0), 
		m_tmLastTime(0), 
		m_nSelectedMenuItemId(0), 
		m_hParent(NULL),
		m_nShownMenuItemId(0)
	{
		SystemParametersInfo(SPI_GETMENUSHOWDELAY, 0, &MENU_TOOLTIP_DELAY, 0);
		MENU_TOOLTIP_TIMER_INTERVAL = MENU_TOOLTIP_DELAY / 4;
	}

	/**/
	HMENU GetTooltipMenu() const
	{
		return NULL;
	}

	_tstring GetTooltipText(int nSelectedMenuItemId) const
	{
		return _tstring();
	}

	/**/
	void HideTooltip()
	{
		if (::IsWindowVisible(m_tooltip.m_hWnd))
		{
			CToolInfo ti(TTF_IDISHWND, m_hParent, (UINT)m_hParent);
			m_tooltip.TrackActivate(&ti, FALSE);
		}
	}

	/**/
	void ShowTooltip()
	{
		if (!::IsWindowVisible(m_tooltip.m_hWnd))
		{
			T* pT = static_cast<T*>(this);
			_tstring sNoteText = pT->GetTooltipText(m_nSelectedMenuItemId);
			if (!sNoteText.empty())
			{
				m_nShownMenuItemId = m_nSelectedMenuItemId;
				CToolInfo ti(TTF_IDISHWND, m_hParent, (UINT)m_hParent, NULL, &sNoteText[0]);
				m_tooltip.UpdateTipText(&ti);
				CPoint pt;
				if (m_rcSelected.PtInRect(m_ptLastCursorPos))
				{
					pt = m_ptLastCursorPos;
					pt.y += 22;
				}
				else
				{
					pt.x = m_rcSelected.left + m_rcSelected.Width() / 2;
					pt.y = m_rcSelected.bottom + 2;
				}
				m_tooltip.TrackPosition(pt.x, pt.y);
				m_tooltip.TrackActivate(&ti, TRUE);
			}
		}
	}

	/**/
	BOOL BeforeTooltipShowing()
	{
		return TRUE;
	}

	/**/
	BEGIN_MSG_MAP(CMenuTooltip)
		m_hParent = hWnd;
		MESSAGE_HANDLER(WM_INITMENUPOPUP, OnInitMenuPopup)
		MESSAGE_HANDLER(WM_UNINITMENUPOPUP, OnUnInitMenuPopup)
		NOTIFY_CODE_HANDLER(TTN_SHOW, OnNotifyCodeHandler)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_MENUSELECT, OnMenuSelect)
	END_MSG_MAP()

private:

	/**/
	LRESULT OnInitMenuPopup(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CMenuHandle menuPopup = (HMENU)wParam;
		UINT nIndex = (UINT)LOWORD(lParam);
		BOOL bSysMenu = (BOOL)HIWORD(lParam);

		T* pT = static_cast<T*>(this);
		CMenuHandle menu(pT->GetTooltipMenu());
		if (menu.m_hMenu != NULL && menuPopup.m_hMenu == menu.m_hMenu)
		{
			m_tooltip.Create(m_hParent, CWindow::rcDefault, NULL, 
				WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP | TTS_NOANIMATE | TTS_NOFADE , 
				WS_EX_TOPMOST);
			m_tooltip.AddTool(&CToolInfo( TTF_IDISHWND | TTF_TRACK | TTF_ABSOLUTE | TTF_TRANSPARENT, m_hParent, (UINT)m_hParent));
			m_tooltip.SetMaxTipWidth(300);
			m_nTimer = SetTimer(m_hParent, MENU_TOOLTIP_TIMER_ID, MENU_TOOLTIP_TIMER_INTERVAL, NULL);
		}
		bHandled = FALSE;
		return 0;
	}

	/**/
	LRESULT OnUnInitMenuPopup(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		UINT nID = (UINT)HIWORD(lParam);
		CMenuHandle menuPopup = (HMENU)wParam;

		T* pT = static_cast<T*>(this);
		CMenuHandle menu(pT->GetTooltipMenu());
		if (menu.m_hMenu != NULL && menuPopup.m_hMenu == menu.m_hMenu)
		{
			KillTimer(m_hParent, m_nTimer);
			m_tooltip.DestroyWindow();
		}
		bHandled = FALSE;
		return 0;
	}

	/* TTN_SHOW */
	LRESULT OnNotifyCodeHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
	{
		if (m_tooltip.m_hWnd == pnmh->hwndFrom)
		{
			CWindowRect rc(m_tooltip);
			CRect rcDesc;
			::GetWindowRect(GetDesktopWindow(), &rcDesc);

			if (rc.right > rcDesc.right)
			{
				int d = rc.right - rcDesc.right;
				if (d > rc.left)
				{
					d = rc.left;
				}
				rc.left -= d;
			}
			if (rc.bottom > rcDesc.bottom)
			{
				int y = m_ptLastCursorPos.y;
				if (!m_rcSelected.PtInRect(m_ptLastCursorPos))
				{
					y = m_rcSelected.top;
				}
				rc.top = y - rc.Height() - 2;
				if (rc.top < 0)
				{
					rc.top = 0;
				}
			}
 			SetWindowPos(m_tooltip,
 				HWND_TOPMOST,
 				rc.left, rc.top,
 				0, 0,
 				SWP_NOSIZE|SWP_NOOWNERZORDER|/*SWP_NOZORDER|*/SWP_NOACTIVATE);

			return 1;
		}
		bHandled = FALSE;
		return 0;
	}

	/**/
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		UINT_PTR nIDEvent = (UINT_PTR)wParam;

		if (nIDEvent == MENU_TOOLTIP_TIMER_ID && m_nSelectedMenuItemId)
		{
			CPoint ptCursorPos;
			::GetCursorPos(&ptCursorPos);
			DWORD nTime = ::GetTickCount();
			if (ptCursorPos != m_ptLastCursorPos)
			{
				m_ptLastCursorPos = ptCursorPos;
				m_tmLastTime = nTime;
				if (m_nShownMenuItemId != m_nSelectedMenuItemId)
				{
					HideTooltip();
				}
			}
			else if (nTime - m_tmLastTime >= MENU_TOOLTIP_DELAY)
			{
				if (m_nShownMenuItemId != m_nSelectedMenuItemId)
				{
					m_ptLastCursorPos = ptCursorPos;
					m_tmLastTime = nTime;
					T* pT = static_cast<T*>(this);
					if (pT->BeforeTooltipShowing())
					{
						ShowTooltip();
					}
				}
			}
		}
		bHandled = FALSE;
		return 0;
	}

	/**/
	LRESULT OnMenuSelect(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		UINT nItemID = (UINT)LOWORD(wParam);
		UINT nFlags = (UINT)HIWORD(wParam);
		CMenuHandle menu = (HMENU)lParam;

		HideTooltip();
		m_nShownMenuItemId = 0;
		if (nFlags & MF_POPUP)
		{
			m_nSelectedMenuItemId = 0;
		}
		else
		{
			m_nSelectedMenuItemId = nItemID;
			for(int nItem = 0; nItem < ::GetMenuItemCount(menu); nItem++) 
			{
				UINT cmd = ::GetMenuItemID(menu, nItem);
				if(cmd == nItemID) 
				{
					::GetMenuItemRect(NULL, menu, nItem, &m_rcSelected);
				}
			}
		}
		bHandled = FALSE;
		return 0;
	}

	UINT_PTR m_nTimer;
	CToolTipCtrl m_tooltip;
	HWND m_hParent;
	CRect m_rcSelected;
	int m_nSelectedMenuItemId;
	int m_nShownMenuItemId;
	CPoint m_ptLastCursorPos;
	DWORD m_tmLastTime;
	DWORD MENU_TOOLTIP_TIMER_INTERVAL;
	DWORD MENU_TOOLTIP_DELAY;

};
