/**
 * \file ModuleView.cpp
 *
 * \since 2006/02/07
 * \author pavel
 */
#include "StdAfx.h"
#include "ModuleView.h"
#include "ModuleLoader.h"
#include "Defines.h"
#include "ApplicationImpl.h"

//////////////////////////////////////////////////////////////////////////
// local functions

CString GetModuleOptionsPath(DWORD mod_id)
{
	TCHAR szPath[MAX_PATH];
	CApplicationImpl::Get().GetModuleOptionsPath(szPath, mod_id);
	return CString(szPath);
}

//////////////////////////////////////////////////////////////////////////
// CModuleView

CModuleView::CModuleView(MODULE* pModule) 
:	m_pModule(pModule), 
	m_Options(GetModuleOptionsPath(pModule->m_pModule->pModuleInfo->GetId()))
{
}

HWND CModuleView::GetHwnd() const 
{ 
	return m_hWnd; 
}

HWND CModuleView::CreateView(HWND parent)
{
	HWND hwnd = Create(parent, CWindow::rcDefault, 
		NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CONTROLPARENT);
	ATLASSERT(::IsWindow(hwnd));
	return hwnd;
}

LRESULT CModuleView::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	// create list view
	Views::IAssistView* pView = GetModule()->m_pModule->pModuleInfo->CreateListViewObject();
	ATLASSERT(pView != NULL);
	HWND hListView = pView->CreateView(m_hWnd);
	ATLASSERT(::IsWindow(hListView));
	AddChildView(pView);

	// set list window as active. all commands will be sent to this window.
	ActivateChildView(pView); 

	// create detail view

	//m_DetailsPane.Create(m_hWnd);
	//m_DetailsPane.SetTitle(_T("Details"));

	pView = GetModule()->m_pModule->pModuleInfo->CreateDetailViewObject();
	ATLASSERT(pView != NULL);

//	HWND hDetailView = pView->CreateView(m_DetailsPane);
	HWND hDetailView = pView->CreateView(m_hWnd);

	ATLASSERT(::IsWindow(hDetailView));
	AddChildView(pView);

//	m_DetailsPane.SetClient(hDetailView);
//	SetSplitterPanes(hListView, m_DetailsPane);
	SetSplitterPanes(hListView, hDetailView);

	PostMessage(WM_SETUP);

	bHandled = FALSE;
	return 1;
}

LRESULT CModuleView::OnInit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	CClientRect rect(GetHwnd());
	if (m_Options.SplitterPos <= 0 || m_Options.SplitterPos >= (DWORD)rect.Height())
		m_Options.SplitterPos = rect.Height() / 3 * 2;

	SetSinglePaneMode(m_Options.DetailsState ? SPLIT_PANE_NONE : SPLIT_PANE_LEFT);
	SetDefaultActivePane(0);
	SetSplitterPos(m_Options.SplitterPos);
	GetParentView()->OnRefreshed(this, UNM_DETAILS_SHOWN, (WPARAM)m_Options.DetailsState);
	return 0;
}

void CModuleView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_TAB)
	{
		// forward to parent for tab navigation
		::SendMessage(CApplicationImpl::Get().GetMainViewHwnd(), WM_KEYDOWN, VK_TAB, 0);
	}
}

LRESULT CModuleView::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	m_Options.SplitterPos = GetSplitterPos();
	return 0;
}

LRESULT CModuleView::OnViewDetailsPane(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_Options.DetailsState = !m_Options.DetailsState;
	SetSinglePaneMode(m_Options.DetailsState ? SPLIT_PANE_NONE : SPLIT_PANE_LEFT);
	GetParentView()->OnRefreshed(this, UNM_DETAILS_SHOWN, (WPARAM)m_Options.DetailsState);
	return 0;
}

BOOL CModuleView::OnActivate(BOOL bActive)
{
	if (bActive)
		GetParentView()->OnRefreshed(this, UNM_DETAILS_SHOWN, (WPARAM)m_Options.DetailsState);

	return TRUE;
}

void CModuleView::Refresh(int code /*= 0*/, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{
	m_Views.front()->Refresh(code, wParam, lParam);
}

HWND CModuleView::SetFocus()
{
	return m_Views.front()->SetFocus();
}

void CModuleView::OnSetFocus(HWND wParam)
{
	SetActivePane(0);
}