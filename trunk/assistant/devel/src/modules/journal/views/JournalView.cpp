/**
 * @file JournalView.cpp
 *
 * CJournalView implementation.
 *
 * @author Pavel Mosunov
 * @since 2004-9-7
 */

#include "StdAfx.h"
#include "resource.h"
//#include "ViewDefs.h"
//#include "ViewTypes.h"
#include "JournalView.h"

namespace Views
{

//////////////////////////////////////////////////////////////////////////
// CJournalView

CJournalView::CJournalView(/*IAssistView* pFrame /*= NULL*/) 
//:	CAssistViewImpl(pFrame) 
{
}

CJournalView::~CJournalView()
{
}

/**
* CJournalView::CreateView()
*/
HWND CJournalView::CreateView(HWND parent)
{
	return Create(parent, CWindow::rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |WS_CLIPCHILDREN);
}

/**
 * WM_CREATE handler
 */
LRESULT CJournalView::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	m_HorizontalSplitter.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |WS_CLIPCHILDREN);
	m_HorizontalSplitter.m_bFullDrag = false; // To get the ghost bar
	
	int viewStyleEx = WS_EX_CLIENTEDGE;
	
	m_JournalListPane.Create(m_HorizontalSplitter, rcDefault,_T("Journal List"), WS_CHILD | WS_VISIBLE, viewStyleEx );
	m_JournalPropsPane.Create(m_HorizontalSplitter, rcDefault,_T("Journal Props"), WS_CHILD | WS_VISIBLE, viewStyleEx );

	m_HorizontalSplitter.SetSplitterPanes(m_JournalListPane, m_JournalPropsPane);

	PostMessage(WM_SETUP, 0, 0);
	return 0;
}

/**
 * WM_SETUP user message handler
 */
LRESULT CJournalView::OnSetup(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CRect rectClient;
	m_HorizontalSplitter.GetClientRect(&rectClient);
	m_HorizontalSplitter.SetSplitterPos(rectClient.Height() /2);
	return 0;
}

/**
 * WM_SIZE handler
 */
LRESULT CJournalView::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CRect rc;
	GetClientRect(&rc);
	m_HorizontalSplitter.MoveWindow(&rc);
	return 0;
}

} //namespace Views

