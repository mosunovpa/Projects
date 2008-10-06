/**
 * @file NoteListPane.h
 *
 * Note list view window.
 *
 * @author Pavel Mosunov
 * @since 2004-09-02
 */

#ifndef __NOTELISTPANE_H__
#define __NOTELISTPANE_H__

#include "ListCtrlWindowImpl.h"

namespace Views
{

/**
 * CNoteListPane - Note list view class.
 */
class CNoteListPane : public CListCtrlWindow
{
public:
	DECLARE_WND_CLASS(_T("NoteList"))

	CNoteListPane();

	BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_MSG_MAP(CNoteListPane)
		CHAIN_MSG_MAP(CListCtrlWindow)
	END_MSG_MAP()

	virtual void Refresh(int code = 0, WPARAM wParam = 0, LPARAM lParam = 0);

private:
	virtual void AddColumns();
	virtual void AddBitmaps();
};

} // namespace Views

#endif //__NOTELISTPANE_H__
