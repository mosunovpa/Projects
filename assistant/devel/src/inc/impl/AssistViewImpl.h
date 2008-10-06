/*!
 *	\file AssistViewImpl.h
 *
 *	\author Pavel Mosunov 
 *  \since 2005/10/04
 */

#ifndef __ASSISTVIEWIMPL_H__
#define __ASSISTVIEWIMPL_H__

#include "assistView.h"
#include "safeptr.h"

#define ACTIVE_CHILD_ID 0x00000001

namespace Views
{
	typedef std::list<IAssistView*> CViewList;

/**
* 
*/
class CAssistViewImpl : public IAssistView, public CDynamicChain
{
public:
	CAssistViewImpl(IAssistView* pParent = NULL) : m_pCurrentView(NULL), m_pParent(pParent) {}
	virtual ~CAssistViewImpl() 
	{
		for (Views::CViewList::iterator it = m_Views.begin(); it != m_Views.end(); ++it)
		{
			(*it)->Delete();
		}
	}

	// child view interface

	virtual void SetParentView(IAssistView* pParent) { m_pParent = pParent; }
	virtual IAssistView* GetParentView() { return m_pParent; }

	virtual BOOL ActivateChildView(IAssistView* pView)
	{
		if (!m_pCurrentView.is_null())
		{
			if (m_pCurrentView.get() == pView)
				return FALSE;

			if (!m_pCurrentView->OnActivate(FALSE)) // deactivate current view
				return FALSE;

			RemoveChainEntry(ACTIVE_CHILD_ID);
			OnChildViewActivated(FALSE, m_pCurrentView);
		}

		CViewList::iterator it =  std::find(m_Views.begin(), m_Views.end(), pView);
		if (it == m_Views.end())
			return FALSE;

		if (!pView->OnActivate(TRUE)) // activate new view
			return FALSE;

		m_pCurrentView = pView;
		SetChainEntry(ACTIVE_CHILD_ID, pView); 
		OnChildViewActivated(TRUE, pView);

		return TRUE;
	}

	virtual void AddChildView(IAssistView* pView)
	{
		m_Views.push_back(pView);
		pView->SetParentView(this);
	}

	virtual IAssistView* GetCurrentChildView() { return m_pCurrentView.get(); }
	virtual HWND SetFocus() { return ::SetFocus(GetHwnd()); }
	virtual void Refresh(int code = 0, WPARAM wParam = 0, LPARAM lParam = 0) {}

	// message map
	BEGIN_MSG_MAP(IAssistView)
	{
		if (uMsg == WM_COMMAND) // go to child view message map
			CHAIN_MSG_MAP_DYNAMIC(ACTIVE_CHILD_ID)
	}
	END_MSG_MAP()

		// virtual interface

	virtual HWND GetHwnd() const = 0;
	virtual HWND CreateView(HWND parent) = 0;
	virtual void Delete() { delete this; }

	// events
	virtual void OnChildViewActivated(BOOL bActive, IAssistView* pView) {}
	virtual BOOL OnActivate(BOOL bActive) 	{ return TRUE; }
	virtual void OnRefreshed(IAssistView* pView, int code, WPARAM wParam = 0, LPARAM lParam = 0) 
	{
		if (m_pParent)
		{
			m_pParent->OnRefreshed(pView, code, wParam, lParam);
		}
	}

protected:
	safe_ptr<IAssistView> m_pParent;						// parent view
	CViewList m_Views;							// child views
	safe_ptr<IAssistView> m_pCurrentView;				// current child view
};

} //namespace Views

#endif //__ASSISTVIEWIMPL_H__
