#pragma once
#include "resource.h"

class CNoteSettingPage : public CPropertyPageImpl<CNoteSettingPage>
{
public:
	enum { IDD = IDD_NOTE_SETTING_PAGE };

	BEGIN_MSG_MAP_EX(CNoteSettingPage)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		CHAIN_MSG_MAP(CPropertyPageImpl<CNoteSettingPage>)
	END_MSG_MAP_EX()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	int OnApply();

};
