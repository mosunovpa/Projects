#pragma once
#include "NoteSettingPage.h"
#include "NotebookSettingPage.h"
#include "SyncSettingPage.h"
#include "KeyboardSettingPage.h"
#include "user_messages.h"

class CSettingsSheet : public CPropertySheetImpl<CSettingsSheet>
{
public:
	CSettingsSheet( _U_STRINGorID title = (LPCTSTR) NULL, 
		UINT uStartPage = 0, HWND hWndParent = NULL );

	BEGIN_MSG_MAP_EX(CSettingsSheet)
		MSG_WM_SHOWWINDOW(OnShowWindow)
		MESSAGE_HANDLER_EX(WMU_SHEET_INIT, OnInit)
		MSG_WM_DESTROY(OnDestroy)
		CHAIN_MSG_MAP(CPropertySheetImpl<CSettingsSheet>)
	END_MSG_MAP_EX()

	void OnShowWindow(BOOL bShow, UINT nStatus);
	void OnSheetInitialized();
	LRESULT OnInit ( UINT uMsg, WPARAM wParam ,LPARAM lParam );
	void OnDestroy();

private:
	CSyncSettingPage m_pageSyncSettings;
	CNotebookSettingPage m_pageNotebookSettings;
	CKeyboardSettingPage m_pageKeybSettings;
	CNoteSettingPage m_pageNoteSettings;
};
