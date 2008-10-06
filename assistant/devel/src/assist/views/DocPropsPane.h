/**
 * @file DocPropsPane.h
 *
 * Note properties view window.
 *
 * @author Pavel Mosunov
 * @since 2004-09-02
 */

#ifndef __DOCPROPSPANE_H__
#define __DOCPROPSPANE_H__

#include <atlcrack.h>
#include "AssistViewImpl.h"

/**
 * CDocPropsPane - Note list view class.
 */
class CDocPropsPane : public CWindowImpl<CDocPropsPane>, public Views::CAssistViewImpl
{
public:
	DECLARE_WND_CLASS_EX(_T("DocProps"), CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, COLOR_APPWORKSPACE);

	BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_MSG_MAP(CDocPropsPane)
//		MESSAGE_HANDLER(WM_PAINT, OnPaint)
	END_MSG_MAP()

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL&	/*bHandled*/);

	virtual HWND GetHwnd() const;
	virtual HWND CreateView(HWND parent);
};

#endif //__DOCPROPSPANE_H__
