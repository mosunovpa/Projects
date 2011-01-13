#include "StdAfx.h"
#include "NewLabelDialog.h"
#include <boost/algorithm/string/trim.hpp>
#include "atlwinmisc.h"
#include "winutils.h"
#include "Application.h"


CNewLabelDialog::CNewLabelDialog(void) : m_nInitParam(ipNone)
{
}

_tstring CNewLabelDialog::GetEditText()
{
	_tstring s;
	s.resize(512, 0);
	m_editCtl.GetWindowText(&s[0], 512);
	boost::trim(s);
	if (s[0] == 0)
	{
		s.clear();
	}
	return s.c_str();
}

_tstring CNewLabelDialog::GetListText(int idx)
{
	_tstring buf;
	int len = m_listCtl.GetTextLen(idx) + 1;
	buf.resize(len);
	m_listCtl.GetText(idx, &buf[0]);
	return buf.c_str();
}

LRESULT CNewLabelDialog::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	m_listCtl = GetDlgItem(IDC_LABELS);
	m_editCtl = GetDlgItem(IDC_EDIT_LABEL);

	if ((m_nInitParam & ipCursorPos) == ipCursorPos)
	{
		CPoint pt;
		GetCursorPos(&pt);
		CWindowRect rc(*this);
		rc.MoveToXY(pt.x - rc.Width() / 2, pt.y - rc.Height() / 2);
		winutils::AdjustScreenRect(rc);
		MoveWindow(rc);
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
	
	m_editCtl.SetWindowText(m_sLabel.c_str());

	std::list<_tstring> listLabels;
	CApplication::Get().GetLabels(listLabels);

	for (std::list<_tstring>::iterator it = listLabels.begin();
		it != listLabels.end(); ++it)
	{
		m_listCtl.AddString(it->c_str());
	}

	FindLabelInList(m_sLabel.c_str());

	return TRUE;
}

LRESULT CNewLabelDialog::OnClear(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_sLabel.clear();
	EndDialog(IDOK);
	return 0;
}

LRESULT CNewLabelDialog::OnOK(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_sLabel = GetEditText();
	EndDialog(IDOK);
	return 0;
}

LRESULT CNewLabelDialog::OnCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(IDCANCEL);
	return 0;
}

void CNewLabelDialog::OnActivate(UINT nState, BOOL bMinimized, CWindow wndOther)
{
	if ((nState == WA_INACTIVE) && ((m_nInitParam & ipPopup) == ipPopup))
	{
		PostMessage(WMU_CLOSE_FORM);
	}
}

LRESULT CNewLabelDialog::OnCloseForm(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	EndDialog(IDCANCEL);
	return 0;
}

void CNewLabelDialog::OnLabelsNotify(UINT uNotifyCode, int nID, CListBox wndCtl)
{
	if (uNotifyCode == LBN_SELCHANGE)
	{
		int selIndex = wndCtl.GetCurSel();
		if (selIndex != LB_ERR)
		{
			_tstring buf = GetListText(selIndex);
			m_editCtl.SetWindowText(buf.c_str());
		}
	}
}

void CNewLabelDialog::OnEditLabelNotify(UINT uNotifyCode, int nID, CEdit wndCtl)
{
	if (uNotifyCode == EN_CHANGE)
	{
		FindLabelInList(GetEditText().c_str());
	}
}

void CNewLabelDialog::FindLabelInList(LPCTSTR str)
{
	int idx = m_listCtl.FindString(0, str);
	if (idx == LB_ERR) 
	{
		m_listCtl.SetCurSel(-1);
	}
	else
	{
		_tstring listText = GetListText(idx);
		if (listText == _tstring(str))
		{
			m_listCtl.SetCurSel(idx);
		}
		else
		{
			m_listCtl.SetCurSel(-1);
		}
	}
}
