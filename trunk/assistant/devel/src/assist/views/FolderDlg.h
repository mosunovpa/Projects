/**
 * \file FolderDlg.h
 *
 * \since 2006/12/11
 * \author pavel
 */

#pragma once

#include "resource.h"
#include "atlddx.h"

class CFolderDlg : public CDialogImpl<CFolderDlg>, public CWinDataExchange<CFolderDlg>
{
public:
	enum { IDD = IDD_FOLDER_DLG };
	CString m_csFolderName;

	static int GetFolderName(CString& csFolderName);

	BEGIN_MSG_MAP(CAboutDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOkCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CAboutDlg)
		DDX_TEXT(IDC_FOLDER_NAME, m_csFolderName)
	END_DDX_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOkCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};