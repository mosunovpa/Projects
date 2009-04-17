#pragma once

class CNoteWnd;

class CStaticSysIcon : public CWindowImpl<CStaticSysIcon, CStatic>  
{
public:
	CStaticSysIcon(CNoteWnd* pNoteWnd);
	~CStaticSysIcon(void);

	DECLARE_WND_SUPERCLASS(NULL, CStatic::GetWndClassName())

	BEGIN_MSG_MAP_TRY(CStaticSysIcon)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_LBUTTONDBLCLK(OnLButtonDblClk)
	END_MSG_MAP_CATCH()

	void ShowMenu(CPoint pt);

private:
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonDblClk(UINT nFlags, CPoint point);

	CNoteWnd* m_pNoteWnd;
};
