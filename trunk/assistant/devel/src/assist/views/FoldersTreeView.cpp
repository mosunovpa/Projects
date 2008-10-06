/**
 * \file FoldersTreeView.cpp
 *
 * \since 2006/07/17
 * \author pavel
 */
#include "stdafx.h"
#include "FoldersTreeView.h"

//////////////////////////////////////////////////////////////////////////
//

LRESULT CFoldersTreeCtrl::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
//	theApp.RegisterView(m_hWnd);
	bHandled = FALSE;
	return 1;
}

LRESULT CFoldersTreeCtrl::OnUpdate(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
// 	switch (wParam)
// 	{
// 	case UNM_FILE_OPENED:
// 		ReloadTree();
// 		break;
// 	}
	return 0;
}

void CFoldersTreeCtrl::ReloadTree()
{
// 	DeleteAllItems();
// 	shared_ptr_2<IStatement> pFolders = theApp.GetDataFile()->GetStatement();
// 	pFolders->sql(_T("select name from adx_folders where parent = 0"));
// 	LPCTSTR szName = NULL;
// 	pFolders->into(&szName, _T("name"));
// 	while (pFolders->exec())
// 	{
// 		InsertItem(szName, TVI_ROOT, TVI_LAST);
// 	}

// 	shared_ptr_2<Models::IFolders> pFolders = theApp.GetDataFile()->GetFolders();
// 	pFolders->select(0);
// 	while (Models::IFolder* pFolder = pFolders->next())
// 	{
// 		_tstring szName = pFolder->GetName();
// 		InsertItem(szName.c_str(), TVI_ROOT, TVI_LAST);
// 	}
}