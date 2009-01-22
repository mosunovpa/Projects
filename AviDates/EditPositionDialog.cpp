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
	GetDlgItem(IDC_POSITION)->SetWindowText(ToString(m_pos));
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

void CEditPositionDialog::SetPosition( CPositionInMovie p )
{
	m_pos = p;
}

CPositionInMovie CEditPositionDialog::GetPosition() const
{
	return m_pos;
}

// CEditPositionDialog message handlers

BOOL CheckFormat(CString cs)
{
	
	return TRUE;
}

CPositionInMovie ToPosition(CString cs)
{
	int h = 
	int m = 
	int s = 
	int 
	return CPositionInMovie(CTimeSpan(0, h, m, s), f);
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
		AfxMessageBox(_T("Not correct position."));
	}
}
