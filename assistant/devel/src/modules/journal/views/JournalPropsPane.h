/**
 * @file JournalPropsPane.h
 *
 * Journal properties view window.
 *
 * @author Pavel Mosunov
 * @since 2004-09-02
 */

#ifndef __JOURNALPROPSPANE_H__
#define __JOURNALPROPSPANE_H__

#include "AssistViewImpl.h"

namespace Views
{

/**
 * CJournalPropsPane - Journal list view class.
 */
class CJournalPropsPane : public CWindowImpl<CJournalPropsPane>, public CAssistViewImpl
{
public:
	virtual ~CJournalPropsPane();

	DECLARE_WND_CLASS(_T("JournalProps"))

	BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_MSG_MAP(CJournalPropsPane)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
	END_MSG_MAP()

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL&	/*bHandled*/);

	virtual HWND GetHwnd() const;
	virtual HWND CreateView(HWND parent);

};

} // namespace Views

#endif //__JOURNALPROPSPANE_H__
