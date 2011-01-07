#pragma once
#include "resource.h"

class CSyncSettingPage : public CPropertyPageImpl<CSyncSettingPage>
{
public:
	enum { IDD = IDD_SYNC_SETTING_PAGE };

	BEGIN_MSG_MAP_EX(CSyncSettingPage)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		CHAIN_MSG_MAP(CPropertyPageImpl<CSyncSettingPage>)
	END_MSG_MAP_EX()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	int OnApply();

};