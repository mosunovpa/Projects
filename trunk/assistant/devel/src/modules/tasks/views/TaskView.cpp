/**
 * @file TaskView.cpp
 *
 * CTaskView implementation.
 *
 * @author Pavel Mosunov
 * @since 2004-9-7
 */

#include "StdAfx.h"
#include "resource.h"
//#include "ViewDefs.h"
//#include "ViewTypes.h"
#include "TaskView.h"


namespace Views
{

//////////////////////////////////////////////////////////////////////////
// CTaskView

CTaskView::CTaskView(/*IAssistView* pFrame /*= NULL*/) 
//:	CAssistViewImpl(pFrame)
{
}

/**
 * CTaskView::~CTaskView()
 */
CTaskView::~CTaskView()
{
}

/**
 * CTaskView::CreateView()
 */
HWND CTaskView::CreateView(HWND parent)
{
	return Create(parent, CWindow::rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |WS_CLIPCHILDREN);
}

/**
 * WM_CREATE handler
 */
LRESULT CTaskView::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	m_HorizontalSplitter.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |WS_CLIPCHILDREN);
	m_HorizontalSplitter.m_bFullDrag = false; // To get the ghost bar
	
	int viewStyleEx = WS_EX_CLIENTEDGE;
	
	m_TaskListPane.Create(m_HorizontalSplitter, rcDefault,_T("Task List"), WS_CHILD | WS_VISIBLE, viewStyleEx );
	m_TaskPropsPane.Create(m_HorizontalSplitter, rcDefault,_T("Task Props"), WS_CHILD | WS_VISIBLE, viewStyleEx );

	m_HorizontalSplitter.SetSplitterPanes(m_TaskListPane, m_TaskPropsPane);

	PostMessage(WM_SETUP, 0, 0);
	return 0;
}

LRESULT CTaskView::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

/**
 * WM_SETUP user message handler
 */
LRESULT CTaskView::OnSetup(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CRect rectClient;
	m_HorizontalSplitter.GetClientRect(&rectClient);
	m_HorizontalSplitter.SetSplitterPos(rectClient.Height() /2);
	return 0;
}

/**
 * WM_SIZE handler
 */
LRESULT CTaskView::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CRect rc;
	GetClientRect(&rc);
	m_HorizontalSplitter.MoveWindow(&rc);
	return 0;
}

} //namespace Views


