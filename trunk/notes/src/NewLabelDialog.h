#pragma once
#include "resource.h"
#include "atlwin.h"
#include "user_messages.h"

class CNewLabelDialog : public CDialogImpl<CNewLabelDialog>
{
public:
	enum InitParam 
	{
		ipNone = 0,
		ipCursorPos = 1,
		ipPopup = 2
	};
	UINT m_nInitParam;

	CNewLabelDialog(void);

	_tstring m_sLabel;

	enum { IDD = IDD_NEW_LABEL };

	BEGIN_MSG_MAP_EX(CNewLabelDialog)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WMU_CLOSE_FORM, OnCloseForm)
		MSG_WM_ACTIVATE(OnActivate)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP_EX()

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCloseForm(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	void OnActivate(UINT nState, BOOL bMinimized, CWindow wndOther);
};
