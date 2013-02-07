#include "StdAfx.h"
#include "NotebookSettingPage.h"
#include "Application.h"
#include "resutils.h"

LRESULT CNotebookSettingPage::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	m_listCtl = GetDlgItem(IDC_NOTEBOOK_LIST);
	m_listCtl.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	m_listCtl.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 240);
//	m_listCtl.AddColumn(_T("File"), 1);

	HANDLE hFind;
	WIN32_FIND_DATA FindData;

	_tstring folderMask = ( CApplication::Get().GetAppFolder() + _T("\\*") );
	hFind = FindFirstFile(folderMask.c_str(), &FindData);
	if (hFind != INVALID_HANDLE_VALUE) 
	{
		while (FindNextFile(hFind, &FindData) != 0) 
		{
			if ((FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
			{
				m_listCtl.AddItem(0, 0, FindData.cFileName);
			}
		}
		FindClose(hFind);
	}


	return TRUE;
}

int CNotebookSettingPage::OnApply()
{

	return PSNRET_NOERROR;
}
