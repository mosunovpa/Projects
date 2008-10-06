/**
 * @file DocListPane.h
 *
 * Note list view window.
 *
 * @author Pavel Mosunov
 * @since 2006-10-24
 */

#ifndef __DOCLISTPANE_H__
#define __DOCLISTPANE_H__

#include "ListCtrlWindowImpl.h"
#include "rsettings.h"
#include "assistres.h"

//////////////////////////////////////////////////////////////////////////
// CDocListPane

/**
 * CDocListPane - Note list view class.
 */
class CDocListPane : public Views::CListCtrlWindow
{
public:
	DECLARE_WND_CLASS(_T("DocList"))

	CDocListPane();
	virtual ~CDocListPane();

	BEGIN_MSG_MAP(CDocListPane)
		CHAIN_MSG_MAP(Views::CListCtrlWindow)
	END_MSG_MAP()

	virtual void Refresh(int code = 0, WPARAM wParam = 0, LPARAM lParam = 0);
	virtual void AddColumns();
	virtual void AddBitmaps();

private:
	std::map<DWORD, int> m_mapModuleImages;
};

#endif //__DOCLISTPANE_H__
