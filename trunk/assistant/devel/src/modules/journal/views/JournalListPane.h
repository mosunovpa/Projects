/**
 * @file JournalListPane.h
 *
 * Journal list view window.
 *
 * @author Pavel Mosunov
 * @since 2004-09-02
 */

#ifndef __JOURNALLISTPANE_H__
#define __JOURNALLISTPANE_H__

#include "AssistViewImpl.h"

namespace Views
{

/**
 * CJournalListPane - Journal list view class.
 */
class CJournalListPane : public CWindowImpl<CJournalListPane>, public CAssistViewImpl
{
public:
	virtual ~CJournalListPane();

	DECLARE_WND_CLASS(_T("JournalList"))

	BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_MSG_MAP(CJournalListPane)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
	END_MSG_MAP()

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL&	/*bHandled*/);

	virtual HWND GetHwnd() const;
	virtual HWND CreateView(HWND parent);
};

} // namespace Views

#endif //__JOURNALLISTPANE_H__
