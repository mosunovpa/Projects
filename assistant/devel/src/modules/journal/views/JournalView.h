/**
 * @file JournalView.h
 *
 * CJournalView interface.
 *
 * @author Pavel Mosunov
 * @since 2004-9-7
 */

#ifndef __JOURNALVIEW_H__
#define __JOURNALVIEW_H__

//#include "atloutbarsplit.h"
#include "JournalListPane.h"
#include "JournalPropsPane.h"
#include "AssistViewImpl.h"
#include "Types.h"
#include "Application.h"

namespace Views
{

class CJournalView : public CWindowImpl<CJournalView>, public CAssistViewImpl
{
public:
	CJournalView(/*IAssistView* pFrame = NULL*/);
	virtual ~CJournalView();

	DECLARE_WND_CLASS(_T("JournalView"))

	BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_MSG_MAP(CJournalView)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_SETUP, OnSetup)
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

	CHorSplitterWindow m_HorizontalSplitter;
	CJournalListPane m_JournalListPane;
	CJournalPropsPane m_JournalPropsPane;

};

} //namespace Views

#endif //__JOURNALVIEW_H__
