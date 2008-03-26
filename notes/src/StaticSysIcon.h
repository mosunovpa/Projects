#pragma once

class CStaticSysIcon : public CWindowImpl<CStaticSysIcon, CStatic>  
{
public:
	CStaticSysIcon(void);
	~CStaticSysIcon(void);

	DECLARE_WND_SUPERCLASS(NULL, CStatic::GetWndClassName())

	BEGIN_MSG_MAP(CStaticSysIcon)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_LBUTTONDBLCLK(OnLButtonDblClk)
	END_MSG_MAP_EX()

private:
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonDblClk(UINT nFlags, CPoint point);

	CMenu m_menuNotes;		// menu displayed when the user clicks on 'Note' icon
};
