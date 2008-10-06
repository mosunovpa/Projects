/**
 * @file DataFileManDlg.h
 *
 * CDataFileManDlg interface.
 *
 * @author Pavel Mosunov
 * @since 2004-9-6
 */

#ifndef __DATAFILEMANDLG_H__
#define __DATAFILEMANDLG_H__

#include "DataFileManDlgOptions.h"

/**
 * class CDataFileManDlg 
 */
class CDataFileManDlg : public CDialogImpl<CDataFileManDlg>, public CDialogResize<CDataFileManDlg>
{
public:
    enum { IDD = IDD_DATAFILEMAN };

	CDataFileManDlg();

	BEGIN_MSG_MAP(CDataFileManDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		CHAIN_MSG_MAP(CDialogResize<CDataFileManDlg>)
	END_MSG_MAP()

	BEGIN_DLGRESIZE_MAP(CDataFileManDlg)
		DLGRESIZE_CONTROL(IDC_DATAFILEDLG_LIST, DLSZ_SIZE_X | DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(IDC_DATAFILEDLG_OPEN, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_DATAFILEDLG_ADD, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_DATAFILEDLG_REMOVE, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_DATAFILEDLG_SETTING, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_DATAFILEDLG_COMMENT_LBL, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_DATAFILEDLG_COMMENTS, DLSZ_MOVE_Y | DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDCANCEL, DLSZ_MOVE_Y | DLSZ_MOVE_X)
	END_DLGRESIZE_MAP()

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

private:
	CDataFileManDlgOptions Options;
};

#endif // __DATAFILEMANDLG_H__