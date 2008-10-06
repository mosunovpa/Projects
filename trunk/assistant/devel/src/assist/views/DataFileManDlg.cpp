/**
 * @file DataFileManDlg.cpp
 *
 * CDataFileManDlg implementation.
 *
 * @author Pavel Mosunov
 * @since 2004-9-6
 */

#include "StdAfx.h"
#include "resource.h"
#include "Globals.h"
#include "atlwinmisc.h"
#include "guiutils.h"

#include "datafilemandlg.h"

/**
 * CDataFileManDlg::CDataFileManDlg()
 */
CDataFileManDlg::CDataFileManDlg() : 
	Options(HKEY_CURRENT_USER,  CString(REG_ROOT) + _T("\\DataFileManager"))
{
	Options.Init();
	Options.Load();
}

/**
 * CAboutDlg::OnInitDialog
 */
LRESULT CDataFileManDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	DlgResize_Init();
	if (WinPosIsValid(Options.Pos))
		MoveWindow(&Options.Pos);
	else
		CenterWindow(GetParent());
	return TRUE;
}

/**
 * CAboutDlg::OnCloseCmd
 */
LRESULT CDataFileManDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	Options.Pos = CWindowRect(m_hWnd);
	Options.Save();
	EndDialog(wID);
	return 0;
}
