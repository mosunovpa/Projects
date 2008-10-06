/**
 * @file TaskPropsPane.h
 *
 * Task properties view window.
 *
 * @author Pavel Mosunov
 * @since 2004-09-02
 */

#ifndef __TASKPROPSPANE_H__
#define __TASKPROPSPANE_H__

#include <atlcrack.h>
#include "AssistViewImpl.h"

namespace Views
{

/**
 * CTaskPropsPane - Task list view class.
 */
class CTaskPropsPane : public CWindowImpl<CTaskPropsPane>, public CAssistViewImpl
{
public:
	DECLARE_WND_CLASS(_T("TaskProps"))

	BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_MSG_MAP(CTaskPropsPane)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
	END_MSG_MAP()

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL&	/*bHandled*/);

	virtual HWND GetHwnd() const;
	virtual HWND CreateView(HWND parent);
};

} // namespace Views

#endif //__TASKPROPSPANE_H__
