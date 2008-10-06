/**
 * @file TaskListPane.h
 *
 * Task list view window.
 *
 * @author Pavel Mosunov
 * @since 2004-09-02
 */

#ifndef __TASKLISTPANE_H__
#define __TASKLISTPANE_H__

#include "ListCtrlWindowImpl.h"

namespace Views
{

/**
 * CTaskListPane - Task list view class.
 */
class CTaskListPane : public CListCtrlWindow
{
public:
	DECLARE_WND_CLASS(_T("TaskList"))

	CTaskListPane();

	BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_MSG_MAP(CTaskListPane)
		CHAIN_MSG_MAP(CListCtrlWindow)
	END_MSG_MAP()

private:
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL&	/*bHandled*/);
	virtual void AddColumns();
	virtual void AddBitmaps();

};

} // namespace Views

#endif //__TASKLISTPANE_H__
