#pragma once
#include "NoteSettingPage.h"

#define UWM_CENTER_SHEET WM_USER + 1

class CSettingsSheet : public CPropertySheetImpl<CSettingsSheet>
{
public:
	CSettingsSheet( _U_STRINGorID title = (LPCTSTR) NULL, 
		UINT uStartPage = 0, HWND hWndParent = NULL );

	BEGIN_MSG_MAP(CSettingsSheet)
		MESSAGE_HANDLER_EX(UWM_CENTER_SHEET, OnInit)
		CHAIN_MSG_MAP(CPropertySheetImpl<CSettingsSheet>)
	END_MSG_MAP()

	void OnSheetInitialized();
	LRESULT OnInit ( UINT uMsg, WPARAM wParam ,LPARAM lParam );

private:

	CNoteSettingPage m_pageNoteSettings;
};
