/**
 * @file NotePropsPane.h
 *
 * Note properties view window.
 *
 * @author Pavel Mosunov
 * @since 2004-09-02
 */

#ifndef __NOTEPROPSPANE_H__
#define __NOTEPROPSPANE_H__

#include <atlcrack.h>
#include "AssistViewImpl.h"

namespace Views
{

/**
 * CNotePropsPane - Note list view class.
 */
class CNotePropsPane : public CWindowImpl<CNotePropsPane>, public CAssistViewImpl
{
public:
	DECLARE_WND_CLASS(_T("NoteProps"))

	BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_MSG_MAP(CNotePropsPane)
		MSG_WM_PAINT(OnPaint)
	END_MSG_MAP()

	void OnPaint(HDC hdc);

	virtual HWND GetHwnd() const;
	virtual HWND CreateView(HWND parent);
};

} // namespace Views

#endif //__NOTEPROPSPANE_H__
