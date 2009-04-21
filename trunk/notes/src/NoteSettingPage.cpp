#include "StdAfx.h"
#include "NoteSettingPage.h"
#include "Application.h"
#include "resutils.h"

LRESULT CNoteSettingPage::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CComboBox cb = GetDlgItem(IDC_COMBO_NOTE_FONT_SIZE);
	cb.AddString(resutils::resstring(IDS_SMALL).c_str());
	cb.AddString(resutils::resstring(IDS_MEDIUM).c_str());
	cb.AddString(resutils::resstring(IDS_LARGE).c_str());

	switch (CApplication::Get().GetOptions().GetFontSize()) 
	{
	case COptions::FS_SMALL:
		cb.SetCurSel(0);
		break;
	case COptions::FS_MEDIUM:
		cb.SetCurSel(1);
		break;
	case COptions::FS_LARGE:
		cb.SetCurSel(2);
		break;
	}

	return TRUE;
}

int CNoteSettingPage::OnApply()
{
	CComboBox cb = GetDlgItem(IDC_COMBO_NOTE_FONT_SIZE);
	switch (cb.GetCurSel())
	{
	case 0:
		CApplication::Get().GetOptions().SetFontSize(COptions::FS_SMALL);
		break;
	case 1:
		CApplication::Get().GetOptions().SetFontSize(COptions::FS_MEDIUM);
		break;
	case 2:
		CApplication::Get().GetOptions().SetFontSize(COptions::FS_LARGE);
		break;
	}

	CApplication::Get().SaveOptions();
	CApplication::Get().OptionsUpdated();
	return PSNRET_NOERROR;
}