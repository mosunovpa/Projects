#include "StdAfx.h"
#include "NotebookSettingPage.h"
#include "Application.h"
#include "resutils.h"

LRESULT CNotebookSettingPage::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CListViewCtrl listCntr = (CListViewCtrl)GetDlgItem(IDC_NOTEBOOK_LIST);
	listCntr.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 240);
//	listCntr.AddColumn(_T("File"), 1);

	HANDLE hFind;
	WIN32_FIND_DATA FindData;

	_tstring folderMask = ( CApplication::Get().GetDataFileFolder() + _T("\\*") );
	hFind = FindFirstFile(folderMask.c_str(), &FindData);
	if (hFind != INVALID_HANDLE_VALUE) 
	{
		while (FindNextFile(hFind, &FindData) != 0) 
		{
			listCntr.AddItem(0, 0, FindData.cFileName);
		}
		FindClose(hFind);
	}
	return TRUE;
}

int CNotebookSettingPage::OnApply()
{

	return PSNRET_NOERROR;
}
