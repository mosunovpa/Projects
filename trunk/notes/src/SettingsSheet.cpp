#include "StdAfx.h"
#include "SettingsSheet.h"

CSettingsSheet::CSettingsSheet( _U_STRINGorID title /*= (LPCTSTR) NULL*/, 
							   UINT uStartPage /*= 0*/, HWND hWndParent /*= NULL*/ )
							   : CPropertySheetImpl<CSettingsSheet> ( title, uStartPage, hWndParent )
{
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	AddPage(m_pageNoteSettings);
}

void CSettingsSheet::OnSheetInitialized()
{
	PostMessage(UWM_CENTER_SHEET);
}

LRESULT CSettingsSheet::OnInit ( UINT uMsg, WPARAM wParam ,LPARAM lParam )
{
	CenterWindow();
	return 0;
}

void CSettingsSheet::OnDestroy()
{

}
