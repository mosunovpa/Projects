/**
 * \file FoldersTreeView.h
 *
 * \since 2006/07/17
 * \author pavel
 */
#ifndef FoldersTreeView_h__
#define FoldersTreeView_h__

#include "Application.h"
#include "guiutils.h"

class CFoldersTreeCtrl : public CWindowImpl<CFoldersTreeCtrl, CTreeViewCtrl>
{
public:
	DECLARE_WND_SUPERCLASS(_T("FoldersTreeCtrl"), CTreeViewCtrl::GetWndClassName())

	BEGIN_MSG_MAP_TRY(CFoldersTreeCtrl)
		// put your message handler entries here
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		//		 MESSAGE_HANDLER(WM_LBUTTONDOWN, OnButtonDown)
		//		 NOTIFY_HANDLER(IDC_FOLDER_TREE, NM_CLICK, OnTreeSelChanged)
		MESSAGE_HANDLER(WM_UPDATE, OnUpdate)
	END_MSG_MAP_CATCH()

	//	LRESULT OnTreeSelChanged(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
	//	{
	//		return 0;
	//	}
	//	LRESULT OnButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	//	{
	//		bHandled = FALSE;
	//		return 0;
	//	}
	//
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnUpdate(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
	void ReloadTree();

};
#endif // FoldersTreeView_h__