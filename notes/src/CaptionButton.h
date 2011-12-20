//////////////////////////////////////////////////////////////////////////////////////////////
// ATL implementation for adding buttons to caption bar
#pragma once

#ifndef __cplusplus
	#error ATL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLBASE_H__
	#error appbar.h requires atlbase.h to be included first
#endif

#ifndef __ATLAPP_H__
	#error appbar.h requires atlapp.h to be included first
#endif

#ifndef __ATLWIN_H__
	#error appbar.h requires atlwin.h to be included first
#endif

#include <vector>
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//	class CCaptionButton
//		Add push buttons to the caption bar
//
//	Author:		Yao Zhifeng
//	Contact:	yaozhifeng@hotmail.com
//
//	Usage:
//		1.use this class as a base class
//		2.use CHAIN_MSG_MAP to chain message to this class
//		3.call AddButton to add one or more buttons to the caption bar
//		4.(IMPORTANT) override GetButtonPos function to provide position for each button added
//		5.handle WM_COMMAND notification sent by caption button in the same way as handle normal button clicking
//		6.(optional) call CheckButton to change the checked status of the caption button
//		7.(optional) call EnableButton to change the enable status of the caption button
//
//	template argument:
//		T: derived class, must also derived from CWindowImpl directly or indirectly
//
template <class T>
class CCaptionButton
{
private:
	struct	_button
	{
		UINT	uID;	//command ID
		int		cx;		//width
		int		cy;		//height
		HIMAGELIST	himl;	//image list
		UINT	uStatus;	//status (image index)
		TCHAR	szHint[80];	//tooltip text
	};

	CToolTipCtrl	m_tooltips;
	vector<_button>	m_buttons;
	int	m_nPressed; //currently pressed button index

public:
	enum
	{
		//Button status /image index
		CAPTION_BTN_NORMAL		= 0,
		CAPTION_BTN_PUSHED		= 1,
		CAPTION_BTN_HOVER		= 2,
		CAPTION_BTN_CHECKED		= 3,
		CAPTION_BTN_DISABLED	= 4,
		CAPTION_BTN_HIDDEN		= 5,

		//Interval between buttons
		CAPTION_BTN_INTERVAL	= 2
	};



	CCaptionButton()
	{
		m_nPressed = -1;
	}

	virtual ~CCaptionButton()
	{
		Clear();
	}


	BEGIN_MSG_MAP(CCaptionButton)
		MESSAGE_HANDLER(WM_NCMOUSELEAVE, OnNcMouseLeave)
		MESSAGE_HANDLER(WM_NCPAINT, OnNcPaint)
		MESSAGE_HANDLER(WM_NCLBUTTONDOWN, OnNcLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		MESSAGE_HANDLER(WM_NCMOUSEMOVE, OnNcMouseMove)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_NCACTIVATE, OnNcActivate)
		NOTIFY_CODE_HANDLER(TTN_GETDISPINFO, OnGetDispInfo)
	END_MSG_MAP()

	/////////////////////////////////////////////////////////////////
	// remove all the buttons
	void Clear()
	{
		while (m_buttons.size())
			RemoveButton(0);
	}

	/////////////////////////////////////////////////////////////
	// remove the button at given position
	void RemoveButton(int index)
	{
		ATLASSERT(index>=0 && index<(int)m_buttons.size());

		vector<_button>::iterator	it = m_buttons.begin();
		for (int i=0; i<index; i++)
			it++;

		//destroy the image list handle
		ImageList_Destroy(it->himl);
		//erase from the list
		m_buttons.erase(it);

		if (m_buttons.empty()) 
		{
			m_tooltips.DestroyWindow();
			m_tooltips.m_hWnd = NULL;
		}

	}

	////////////////////////////////////////////////////////////////
	// append a button
	// return the count of buttons
	int AddButton(UINT uID, int cx, int cy, HIMAGELIST himl, LPCTSTR lpszHint=NULL, 
		UINT uStatus = CAPTION_BTN_NORMAL)
	{
		T* pT = static_cast<T*>(this);

		if (m_tooltips.m_hWnd == NULL)
		{
			m_tooltips.Create(NULL, NULL, NULL, WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
				WS_EX_TOOLWINDOW|WS_EX_TOPMOST);
			ATLASSERT(m_tooltips.m_hWnd);

			TOOLINFO	info;
			memset(&info, 0, sizeof(info));
			info.cbSize		= sizeof(info);
			info.uFlags		= TTF_IDISHWND;
			info.hwnd		= pT->m_hWnd;
			info.uId		= (UINT)pT->m_hWnd;
			info.lpszText	= LPSTR_TEXTCALLBACK;
			m_tooltips.AddTool(&info);

			m_tooltips.Activate(TRUE);
		}

		_button	btn;
		memset(&btn, 0, sizeof(btn));
		btn.uID = uID;
		btn.cx	= cx;
		btn.cy	= cy;
		btn.himl= himl;
		btn.uStatus = uStatus;
		if (lpszHint)
		{
			_tcsncpy(btn.szHint, lpszHint, 80);
		}

		m_buttons.push_back(btn);

		return m_buttons.size();
	}

	///////////////////////////////////////////////////////////////////////////////////
	// check or uncheck the button at postion index
	void CheckButton(int index, bool bCheck)
	{
		T* pT = static_cast<T*>(this);

		_button &btn = m_buttons[index];
		btn.uStatus = bCheck ? CAPTION_BTN_CHECKED : CAPTION_BTN_NORMAL;

		//Update visual effect
		if (pT->m_hWnd)
		{
			//force repaint
			pT->SendMessage(WM_NCPAINT/*, 1*/);
		}
	}

	/////////////////////////////////////////////////////////////////////////////////
	// if the button at position index checked
	bool IsButtonChecked(int index)
	{
		_button &btn = m_buttons[index];

		return (btn.uStatus == CAPTION_BTN_CHECKED);
	}

	/////////////////////////////////////////////////////////////////////////////
	// enable/disable button at given position
	void EnableButton(int index, bool bEnable)
	{
		T* pT = static_cast<T*>(this);

		_button &btn = m_buttons[index];
		btn.uStatus = bEnable ? CAPTION_BTN_NORMAL : CAPTION_BTN_DISABLED;

		//Update visual effect
		if (pT->m_hWnd)
		{
			//force repaint
			pT->SendMessage(WM_NCPAINT/*, 1*/);
		}
	}

	bool IsButtonEnabled(int index)
	{
		_button &btn = m_buttons[index];

		return (btn.uStatus != CAPTION_BTN_DISABLED);
	}

	/////////////////////////////////////////////////////////////////////////////
	// show/hide button at given position
	void ShowButton(int index, bool bVisible, bool bRepaint = false)
	{
		T* pT = static_cast<T*>(this);

		_button &btn = m_buttons[index];
		btn.uStatus = bVisible ? CAPTION_BTN_NORMAL : CAPTION_BTN_HIDDEN;

		//Update visual effect
		if (pT->m_hWnd && bRepaint)
		{
			//force repaint
			pT->SendMessage(WM_NCPAINT/*, 1*/);
		}
	}

	bool IsButtonVisible(int index)
	{
		_button &btn = m_buttons[index];

		return (btn.uStatus != CAPTION_BTN_HIDDEN);
	}

	/////////////////////////////////////////////////////////////////////////////////
	// Get the zero based index of the button, given the button ID
	int GetButtonIndex(UINT uID)
	{
		for (int i=0; i<(int)m_buttons.size(); i++)
		{
			if (m_buttons[i].uID == uID)
				return i;
		}

		return -1;
	}

	/////////////////////////////////////////////////////////////////////////////////
	// Get button size
	SIZE GetButtonSize(int index) 
	{
		_button &btn = m_buttons[index];
		return CSize(btn.cx, btn.cy);
	}

	/////////////////////////////////////////////////////////////////////////////////
	// Get buttons count
	int GetButtonCount()
	{
		return (int)m_buttons.size();
	}

	//////////////////////////////////////////////////////////////////////////////////
	// get the topleft corner of the given button, in window coordinate
	// override this function in the derived class to specify button position
	// this default implementation only works for tool window without any existing caption buttons
	POINT GetButtonPos(int index)
	{
		T* pT = static_cast<T*>(this);
		//get the window rect
		CRect	rcWnd;
		pT->GetWindowRect(&rcWnd);
		rcWnd.OffsetRect(-rcWnd.TopLeft());

		rcWnd.DeflateRect(2, 2);

		//locate the top right base point
		CPoint	pt(rcWnd.right, rcWnd.top);

		//calculate button position
		for (int i=0; i<=index; i++)
		{
			if (IsButtonVisible(i))
			{
				_button &btn = m_buttons[i];
				pt.x -= btn.cx;
				pt.x -= CAPTION_BTN_INTERVAL;
			}
		}

		return pt;
	}

	///////////////////////////////////////////////////////////////////////////////////
	// get the rect of given button in window coordinate
	RECT GetButtonRect(int index)
	{
		T* pT = static_cast<T*>(this);

		_button btn = m_buttons[index];

		POINT	pt = pT->GetButtonPos(index);
		RECT	rc;
		rc.left = pt.x;
		rc.top  = pt.y;
		rc.right = rc.left + btn.cx;
		rc.bottom = rc.top + btn.cy;

		return rc;
	}

	////////////////////////////////////////////////////////////////////////////////////
	// get button under point, in window coordinate
	int GetButtonAtPos(POINT pt)
	{
		T* pT = static_cast<T*>(this);
		for (int i=0; i<(int)m_buttons.size(); i++)
		{
			CRect	rc = pT->GetButtonRect(i);
			if (rc.PtInRect(pt))
				return i;
		}
		return -1;
	}

	////////////////////////////////////////////////////////////////////////////////////
	// draw caption buttons
	void DrawCaptionButtons(HDC hdc)
	{
		T* pT = static_cast<T*>(this);

		for (int i=0; i<(int)m_buttons.size(); i++)
		{
			_button btn = m_buttons[i];
			POINT	pt = pT->GetButtonPos(i);
			if (IsButtonVisible(i))
			{
				ImageList_Draw(btn.himl, btn.uStatus, hdc, pt.x, pt.y, ILD_NORMAL);
			}
		}
	}

private:
	////////////////////////////////////////////////////////////////////////////////////
	// WM_NCHITTEST	 message handler
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		T* pT = static_cast<T*>(this);
		LRESULT hit = ::DefWindowProc(pT->m_hWnd, uMsg, wParam, lParam);

		UpdateStatus();

		CPoint	pt(lParam);
		CRect	rcWnd;
		pT->GetWindowRect(&rcWnd);
		pt.Offset(-rcWnd.TopLeft());

		int nButton = GetButtonAtPos(pt);
		if (nButton != -1 
			&& m_buttons[nButton].uStatus != CAPTION_BTN_DISABLED
			&& m_buttons[nButton].uStatus != CAPTION_BTN_HIDDEN)
		{
			bHandled = TRUE;
		}

		RelayEvent();

		return hit;
	}

	///////////////////////////////////////////////////////////////////////////////////
	// WM_NCPAINT message handler
	// draw the buttons
	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		T* pT = static_cast<T*>(this);
		CWindowDC	dc(pT->m_hWnd);
		DrawCaptionButtons(dc);
		return 0;
	}

	///////////////////////////////////////////////////////////////////////////////////
	// WM_NCLBUTTONDOWN messange handler
	// if pressed on a button, SetCapture if not already captured
	// and push the button
	LRESULT OnNcLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;

		T* pT = static_cast<T*>(this);

		WORD	hit = wParam;
		CPoint	pt(lParam);
		CRect	rcWnd;
		pT->GetWindowRect(&rcWnd);
		pt.Offset(-rcWnd.TopLeft());

		int nButton = GetButtonAtPos(pt);
		if (nButton != -1)
		{
			_button &btn = m_buttons[nButton];
			if (btn.uStatus != CAPTION_BTN_DISABLED && btn.uStatus != CAPTION_BTN_HIDDEN)
			{
				if (::GetCapture() != pT->m_hWnd)
					pT->SetCapture();

				m_nPressed = nButton;
				
				m_tooltips.Activate(FALSE);

				if (btn.uStatus != CAPTION_BTN_CHECKED)
					btn.uStatus = CAPTION_BTN_PUSHED;

				//force repaint
				pT->SendMessage(WM_NCPAINT/*, 1*/);
			}
		}

		return 0;
	}

	///////////////////////////////////////////////////////////////////////////////////
	// WM_MOUSEMOVE message handler
	// 
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		T* pT = static_cast<T*>(this);

		if (::GetCapture() == pT->m_hWnd)
		{
			UpdateStatus();
		}

		return 0;
	}

	///////////////////////////////////////////////////////////////////////////////////
	// WM_NCMOUSEMOVE message handler
	// 
	LRESULT OnNcMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		T* pT = static_cast<T*>(this);

		TRACKMOUSEEVENT tme = { 0 };
		tme.hwndTrack = pT->m_hWnd;
		tme.cbSize = sizeof(tme);
		tme.dwFlags = TME_NONCLIENT | TME_LEAVE;
//		tme.dwHoverTime = 1000 * 3;
		TrackMouseEvent(&tme);

		UpdateStatus();

		RelayEvent();

		return 0;
	}

	///////////////////////////////////////////////////////////////////////////////////
	// WM_NCMOUSELEAVE message handler
	// 
	LRESULT OnNcMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		T* pT = static_cast<T*>(this);
		if (::GetCapture() != pT->m_hWnd)
		{
			UpdateStatus();
		}
		return 0;
	}

	///////////////////////////////////////////////////////////////////////////////
	// WM_LBUTTONUP message handler
	// valid only if mouse captured
	// fire button clicked event if one button clicked
	LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;

		T* pT = static_cast<T*>(this);
		if (::GetCapture() == pT->m_hWnd && m_nPressed != -1)
		{
			//bHandled = TRUE;
			::ReleaseCapture();

			m_tooltips.Activate(TRUE);

			CPoint	pt;
			::GetCursorPos(&pt);
			CRect	rcWnd;
			pT->GetWindowRect(&rcWnd);
			pt.Offset(-rcWnd.TopLeft());

			int nButton = GetButtonAtPos(pt);
			if (m_nPressed != -1 && nButton == m_nPressed)
			{
				//fire click event
				pT->OnCaptionButtonClicked(nButton);
			}

			m_nPressed = -1;
			//force repaint
			//pT->SendMessage(WM_NCPAINT, 1);
			UpdateStatus();
		}

		return 1;
	}

	///////////////////////////////////////////////////////////////////////////////
	// WM_NCACTIVATE messsage handler
	// force repaint of non-client area
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		T* pT = static_cast<T*>(this);
//		pT->SendMessage(WM_NCPAINT, 1);
		return 1;
	}

	///////////////////////////////////////////////////////////////////////////
	// Notify TTN_GETDISPINFO handler
	// provide tooltip text dynamicaly
	LRESULT OnGetDispInfo(int idCtrl, LPNMHDR lpnmHdr, BOOL &bHandled)
	{
		NMTTDISPINFO *lptt = (NMTTDISPINFO *)lpnmHdr;

		T* pT = static_cast<T*>(this);

		CPoint	pt;
		::GetCursorPos(&pt);
		CRect	rcWnd;
		pT->GetWindowRect(&rcWnd);
		pt.Offset(-rcWnd.TopLeft());

		int nButton = pT->GetButtonAtPos(pt);
		if (nButton != -1)
		{
			_button &btn = m_buttons[nButton];

			if (btn.uStatus != CAPTION_BTN_DISABLED && btn.uStatus != CAPTION_BTN_HIDDEN)
				_tcsncpy_s(lptt->szText, 80, btn.szHint, 80);
		}

		lptt->uFlags = TTF_IDISHWND;
		lptt->lpszText = lptt->szText;


		return 0;
	}

protected:
	/////////////////////////////////////////////////////////////////////////////////////
	// fire button cliced event
	// can be override in derived class
	// the default implementation send WM_COMMAND with button ID
	void OnCaptionButtonClicked(int index)
	{
		T* pT = static_cast<T*>(this);

		_button &btn = m_buttons[index];
		pT->SendMessage(WM_COMMAND, MAKELONG(btn.uID, 0), 0);

	}

	///////////////////////////////////////////////////////////////////////////
	// update all button status and repaint
	void UpdateStatus()
	{
		T* pT = static_cast<T*>(this);

		CPoint	pt;
		::GetCursorPos(&pt);
		CRect	rcWnd;
		pT->GetWindowRect(&rcWnd);
		pt.Offset(-rcWnd.TopLeft());

		bool	bRepaint = false;
		for (int i=0; i<(int)m_buttons.size(); i++)
		{
			_button &btn = m_buttons[i];
			
			if (btn.uStatus != CAPTION_BTN_DISABLED && btn.uStatus != CAPTION_BTN_HIDDEN)
			{
				CRect	rc = GetButtonRect(i);
				UINT	uNewStatus;
				if (rc.PtInRect(pt))
				{
					if (btn.uStatus==CAPTION_BTN_CHECKED)
						uNewStatus = CAPTION_BTN_CHECKED;
					else if (m_nPressed == i) //mouse pressed in button
						uNewStatus = CAPTION_BTN_PUSHED;
					else
						uNewStatus = CAPTION_BTN_HOVER;
				}
				else
				{
					if (btn.uStatus!=CAPTION_BTN_CHECKED) //If old status is not checked, set it normal
						uNewStatus = CAPTION_BTN_NORMAL;
					else
						uNewStatus = CAPTION_BTN_CHECKED;
				}

				if (btn.uStatus != uNewStatus)//repaint only if status changed
				{
					bRepaint = true;
					btn.uStatus = uNewStatus;
				}
			}
		}

		if (bRepaint)
		{
			pT->SendMessage(WM_NCPAINT/*, 1*/);
		}
	}

	///////////////////////////////////////////////////////////////////////////////////
	// tooltip processing
	void RelayEvent()
	{
		if (m_tooltips.IsWindow())
		{
			T* pT = static_cast<T*>(this);

			CPoint	pt;
			::GetCursorPos(&pt);
			CRect	rcWnd;
			pT->GetWindowRect(&rcWnd);
			pt.Offset(-rcWnd.TopLeft());

			MSG	msg;
			memset(&msg, 0, sizeof(msg));
			msg.hwnd		= pT->m_hWnd;
			msg.message		= WM_NCMOUSEMOVE;
			msg.lParam		= MAKELONG(pt.x, pt.y);
			msg.pt			= pt;

			static int nLastButton = -1;
			int nButton = GetButtonAtPos(pt);
			if (nButton != nLastButton)//change hovered button
			{
				m_tooltips.Pop();
				nLastButton = nButton;
			}

			m_tooltips.RelayEvent(&msg);
		}
	}


};
