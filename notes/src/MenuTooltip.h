#pragma once

template <class T>
class CMenuTooltip
{
public:
	CMenuTooltip() : 
		m_nTimer(0), 
		m_tmLastTime(0), 
		m_nSelectedMenuItemId(0), 
		m_hParent(NULL)
	{
	}

	HMENU GetTooltipMenu() const
	{
		return NULL;
	}

	_tstring GetTooltipText(int nSelectedMenuItemId) const
	{
		return _tstring();
	}

	void HideTooltip()
	{
		if (::IsWindowVisible(m_tooltip.m_hWnd))
		{
			CToolInfo ti(TTF_IDISHWND, m_hParent, (UINT)m_hParent);
			m_tooltip.TrackActivate(&ti, FALSE);
		}
	}

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
				m_tooltip.TrackPosition(m_ptLastCursorPos.x, m_ptLastCursorPos.y + 22);
				::SetWindowPos(m_tooltip, HWND_TOPMOST ,0,0,0,0, SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOMOVE);
				m_tooltip.TrackActivate(&ti, TRUE);
			}
		}
	}

	BOOL BeforeTooltipShowing()
	{
		return TRUE;
	}

	BEGIN_MSG_MAP_EX(CMenuTooltip)
		m_hParent = hWnd;
		MSG_WM_INITMENUPOPUP(OnInitMenuPopup)
		MSG_WM_UNINITMENUPOPUP(OnUnInitMenuPopup)
		MSG_WM_TIMER(OnTimer)
		MSG_WM_MENUSELECT(OnMenuSelect)
	END_MSG_MAP_EX()

private:

	void OnInitMenuPopup(CMenuHandle menuPopup, UINT nIndex, BOOL bSysMenu)
	{
		T* pT = static_cast<T*>(this);
		CMenuHandle menu(pT->GetTooltipMenu());
		if (menu.m_hMenu != NULL && menuPopup.m_hMenu == menu.m_hMenu)
		{
			m_tooltip.Create(m_hParent, CWindow::rcDefault, NULL, WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP, WS_EX_TOPMOST);
			m_tooltip.AddTool(&CToolInfo( TTF_IDISHWND | TTF_TRACK | TTF_ABSOLUTE | TTF_TRANSPARENT, m_hParent, (UINT)m_hParent));
			m_tooltip.SetMaxTipWidth(300);
			m_nTimer = SetTimer(m_hParent, 654, 100, NULL);
		}
	}

	void OnUnInitMenuPopup(UINT nID, CMenuHandle menuPopup)
	{
		T* pT = static_cast<T*>(this);
		CMenuHandle menu(pT->GetTooltipMenu());
		if (menu.m_hMenu != NULL && menuPopup.m_hMenu == menu.m_hMenu)
		{
			KillTimer(m_hParent, m_nTimer);
			m_tooltip.DestroyWindow();
		}
	}

	void OnTimer(UINT_PTR nIDEvent)
	{
		if (nIDEvent == 654 && m_nSelectedMenuItemId)
		{
			CPoint ptCursorPos;
			::GetCursorPos(&ptCursorPos);
			DWORD nTime = ::GetTickCount();
			if (ptCursorPos != m_ptLastCursorPos)
			{
				m_ptLastCursorPos = ptCursorPos;
				m_tmLastTime = nTime;
				HideTooltip();
			}
			else if (nTime - m_tmLastTime >= 500)
			{
				if (m_nShownMenuItemId != m_nSelectedMenuItemId &&
					m_rcSelected.PtInRect(m_ptLastCursorPos))
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
	}

	void OnMenuSelect(UINT nItemID, UINT nFlags, CMenuHandle menu)
	{
		m_nSelectedMenuItemId = nItemID;
		for(int nItem = 0; nItem < ::GetMenuItemCount(menu); nItem++) {
			UINT cmd = ::GetMenuItemID(menu, nItem);
			if(cmd == nItemID) {
				::GetMenuItemRect(NULL/*m_hParent*/, menu, nItem, &m_rcSelected);
			}
		}
//		::GetMenuItemRect(NULL/*m_hParent*/, menu, nItemID, &m_rcSelected);
	}

	UINT_PTR m_nTimer;
	CToolTipCtrl m_tooltip;
	HWND m_hParent;
	CRect m_rcSelected;
	int m_nSelectedMenuItemId;
	int m_nShownMenuItemId;
	CPoint m_ptLastCursorPos;
	DWORD m_tmLastTime;
};
