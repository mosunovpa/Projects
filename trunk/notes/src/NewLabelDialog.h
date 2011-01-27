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
	BOOL m_bEnableClearButton;

	enum { IDD = IDD_NEW_LABEL };

	BEGIN_MSG_MAP_EX(CNewLabelDialog)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WMU_CLOSE_FORM, OnCloseForm)
		MSG_WM_ACTIVATE(OnActivate)
		COMMAND_ID_HANDLER(IDC_CLEAR, OnClear)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_ID_HANDLER_EX(IDC_LABELS, OnLabelsNotify)
		COMMAND_ID_HANDLER_EX(IDC_EDIT_LABEL, OnEditLabelNotify)
	END_MSG_MAP_EX()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCloseForm(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnClear(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	void OnActivate(UINT nState, BOOL bMinimized, CWindow wndOther);
	void OnLabelsNotify(UINT uNotifyCode, int nID, CListBox wndCtl);
	void OnEditLabelNotify(UINT uNotifyCode, int nID, CEdit wndCtl);

private:
	_tstring GetEditText();
	_tstring GetListText(int idx);
	void FindLabelInList(LPCTSTR str);

	CListBox m_listCtl;
	CEdit m_editCtl;
};
