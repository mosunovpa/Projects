#pragma once
#include "resource.h"

class CSyncSettingPage : public CPropertyPageImpl<CSyncSettingPage>
{
public:
	enum { IDD = IDD_SYNC_SETTING_PAGE };

	BEGIN_MSG_MAP(CSyncSettingPage)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		CHAIN_MSG_MAP(CPropertyPageImpl<CSyncSettingPage>)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	int OnApply();

};