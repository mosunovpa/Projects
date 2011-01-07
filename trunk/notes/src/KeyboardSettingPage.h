#pragma once
#include "resource.h"

class CKeyboardSettingPage : public CPropertyPageImpl<CKeyboardSettingPage>
{
public:
	enum { IDD = IDD_KEYBOARD_SETTING_PAGE };

	BEGIN_MSG_MAP_EX(CKeyboardSettingPage)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		CHAIN_MSG_MAP(CPropertyPageImpl<CKeyboardSettingPage>)
	END_MSG_MAP_EX()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	int OnApply();
};
