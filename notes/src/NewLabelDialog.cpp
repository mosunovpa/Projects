#include "StdAfx.h"
#include "NewLabelDialog.h"
#include <boost/algorithm/string/trim.hpp>
#include "atlwinmisc.h"
#include "winutils.h"

CNewLabelDialog::CNewLabelDialog(void)
{
}

LRESULT CNewLabelDialog::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	if (lParam)
	{
		CenterWindow();
	}
	else
	{
		CWindow wndParent(GetParent());
		if (wndParent.IsWindow())
		{
			CWindowRect rcParent(wndParent);
			CWindowRect rc(*this);
			rc.MoveToXY(rcParent.left + 40, rcParent.top + 40);
			winutils::AdjustScreenRect(rc);
			MoveWindow(rc);
		}
		else
		{
			CenterWindow();
		}
	}
	SetDlgItemText(IDC_EDIT_LABEL, m_sLabel.c_str());
	return TRUE;
}

LRESULT CNewLabelDialog::OnOK(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_sLabel.resize(512, 0);
	GetDlgItemText(IDC_EDIT_LABEL, &m_sLabel[0], 512);
	boost::trim(m_sLabel);
	if (m_sLabel[0] == 0)
	{
		m_sLabel.clear();
	}

	EndDialog(IDOK);
	return 0;
}

LRESULT CNewLabelDialog::OnCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(IDCANCEL);
	return 0;
}
