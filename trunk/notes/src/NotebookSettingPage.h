#pragma once
#include "resource.h"

class CNotebookSettingPage : public CPropertyPageImpl<CNotebookSettingPage>
{
public:
	enum { IDD = IDD_NOTEBOOKS_SETTING_PAGE };

	BEGIN_MSG_MAP(CNotebookSettingPage)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		CHAIN_MSG_MAP(CPropertyPageImpl<CNotebookSettingPage>)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	int OnApply();

};
