#include "StdAfx.h"
#include "SettingsSheet.h"
#include "winutils.h"
#include "atlwinmisc.h"

CSettingsSheet::CSettingsSheet( _U_STRINGorID title /*= (LPCTSTR) NULL*/, 
							   UINT uStartPage /*= 0*/, HWND hWndParent /*= NULL*/ )
							   : CPropertySheetImpl<CSettingsSheet> ( title, uStartPage, hWndParent )
{
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
//	AddPage(m_pageSyncSettings);
//	AddPage(m_pageNotebookSettings);
	AddPage(m_pageKeybSettings);
	//AddPage(m_pageNoteSettings);
}

void CSettingsSheet::OnShowWindow(BOOL bShow, UINT nStatus)
{
	//CenterWindow();
	POINT pt;
	GetCursorPos(&pt);
	CWindowRect rc(m_hWnd);
	rc.MoveToXY(pt);
	winutils::AdjustScreenRect(rc);
	SetWindowPos(HWND_TOP, &rc, SWP_NOSIZE);
}

void CSettingsSheet::OnSheetInitialized()
{
	PostMessage(WMU_SHEET_INIT);
}

LRESULT CSettingsSheet::OnInit ( UINT uMsg, WPARAM wParam ,LPARAM lParam )
{
//	CenterWindow();
	return 0;
}

void CSettingsSheet::OnDestroy()
{

}
