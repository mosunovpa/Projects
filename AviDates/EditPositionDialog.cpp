// EditPositionDialog.cpp : implementation file
//

#include "stdafx.h"
#include "AviDates.h"
#include "EditPositionDialog.h"


// CEditPositionDialog dialog

IMPLEMENT_DYNAMIC(CEditPositionDialog, CDialog)

CEditPositionDialog::CEditPositionDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CEditPositionDialog::IDD, pParent)
	, m_bMoveNextPos(FALSE)
{

}

CEditPositionDialog::~CEditPositionDialog()
{
}

BOOL CEditPositionDialog::OnInitDialog()
{
	GetDlgItem(IDC_POSITION)->SetWindowText(FramesToTimeFormat(m_pos));
	return TRUE;
}

void CEditPositionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_MOVE_NEXT, m_bMoveNextPos);
}


BEGIN_MESSAGE_MAP(CEditPositionDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CEditPositionDialog::OnBnClickedOk)
END_MESSAGE_MAP()

void CEditPositionDialog::SetPosition( int nPos )
{
	m_pos = nPos;
}

int CEditPositionDialog::GetPosition() const
{
	return m_pos;
}

// CEditPositionDialog message handlers

BOOL CheckFormat(CString cs)
{
	int len = cs.GetLength();
	if (len < 10) 
		return FALSE;

	for(int i = 0; i < len; ++i)
	{
		switch (i)
		{
		case 2:
		case 5:
			if (cs[i] != ':') 
				return FALSE;
			break;
		case 8:
			if (cs[i] != '.') 
				return FALSE;
			break;
		default:
			if (!_istdigit(cs[i])) 
				return FALSE;
		}
	}
	return TRUE;
}

int ToPosition(CString cs)
{
	int h = _ttoi(CString((LPCTSTR)cs, 2));
	int m = _ttoi(CString((LPCTSTR)cs + 3, 2));
	int s = _ttoi(CString((LPCTSTR)cs + 6, 2));
	int f = _ttoi((LPCTSTR)cs + 9);
	return f + 25 * (s + m * 60 + h * 3600);
}

void CEditPositionDialog::OnBnClickedOk()
{
	CString csPosition;
	GetDlgItemText(IDC_POSITION, csPosition);
	if (CheckFormat(csPosition))
	{
		m_pos = ToPosition(csPosition);
		OnOK();
	}
	else
	{
		AfxMessageBox(_T("Position is not correct."));
	}
}
