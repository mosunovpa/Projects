/**
 * @file TaskView.h
 *
 * CTaskView interface.
 *
 * @author Pavel Mosunov
 * @since 2004-9-7
 */

#ifndef __TASKVIEW_H__
#define __TASKVIEW_H__

//#include "atloutbarsplit.h"
#include "TaskListPane.h"
#include "TaskPropsPane.h"
#include "AssistViewImpl.h"
#include "Types.h"
#include "Application.h"

namespace Views
{

class CTaskView : public CWindowImpl<CTaskView>, public CAssistViewImpl
{
public:
	CTaskView(/*IAssistView* pFrame = NULL*/);
	virtual ~CTaskView();

	DECLARE_WND_CLASS(_T("TaskView"))

	BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_MSG_MAP(CTaskView)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_SETUP, OnSetup)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
		CHAIN_MSG_MAP(CAssistViewImpl)
	END_MSG_MAP()

	// interface
	virtual HWND GetHwnd() const { return m_hWnd; };
	virtual HWND CreateView(HWND parent);

private:
	// message handlers;
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSetup(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	CHorSplitterWindow m_HorizontalSplitter;
	CTaskListPane m_TaskListPane;
	CTaskPropsPane m_TaskPropsPane;
};

} //namespace Views

#endif //__TASKVIEW_H__
