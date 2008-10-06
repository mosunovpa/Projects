/**
 * @file MainController.cpp
 *
 * CMainController implementation.
 *
 * @author Pavel Mosunov
 * @since 2004-9-6
 */

#include "StdAfx.h"
#include "MainController.h"
#include "resource.h"
#include "Database.h"
#include "AboutDlg.h"
#include "AssistViewImpl.h"
#include "ApplicationImpl.h"
#include "ssfiledialog.h"
#include "GuiUtils.h"

/**
 * Constructor.
 */
CMainController::CMainController() 
{
}

/**
 * Destructor.
 */
CMainController::~CMainController(void)
{
}


/**
 * CMainController::ShowAbout()
 */
void CMainController::ShowAbout()
{
	CAboutDlg dlg;
	dlg.DoModal();
}

LRESULT CMainController::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	return 0;
}

/**
* CMainController::OnAppAbout.
* Show about dialog.
*/
LRESULT CMainController::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ShowAbout();
	return 0;
}

/**
 * CMainController::OnNewDataFile()
 */
LRESULT CMainController::OnNewDataFile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CSSFileDialog dlg(TRUE, _T("*.adx"), NULL, OFN_HIDEREADONLY, CResString<64>(IDS_DATA_FILE_FILTER),
		CApplicationImpl::Get().GetMainViewHwnd());
	if (dlg.DoModal() == IDOK)
	{
		UpdateWindow(CApplicationImpl::Get().GetMainViewHwnd());
		CWaitCursor wc;
		CApplicationImpl::Get().OpenDataFile(dlg.m_szFileName);
		return 1;
	}
	return 0;
}

LRESULT CMainController::OnFileRecent(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CApplicationImpl::Get().OpenDataFile(wID);
	return 0;
}
