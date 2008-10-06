/**
 * \file FolderDlg.cpp
 *
 * \since 2006/12/11
 * \author pavel
 */

#include "stdafx.h"
#include "FolderDlg.h"


//////////////////////////////////////////////////////////////////////////
//
/**
 * static member for showing folder name
 */
int CFolderDlg::GetFolderName(CString& csFolderName)
{
	CFolderDlg dlg;
	dlg.m_csFolderName = csFolderName;
	int res = dlg.DoModal();
	if (res == IDOK)
	{
		csFolderName = dlg.m_csFolderName;
	}
	return res;
}

/**
 * 
 */
LRESULT CFolderDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());
	DoDataExchange();
	return TRUE;
}

LRESULT CFolderDlg::OnOkCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DoDataExchange(TRUE);
	EndDialog(wID);
	return 0;
}

LRESULT CFolderDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}