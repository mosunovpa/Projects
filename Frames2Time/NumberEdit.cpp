// NumberEdit.cpp : implementation file
//

#include "stdafx.h"
#include "Frames2Time.h"
#include "NumberEdit.h"

//////////////////////////////////////////////////////////////////////////
// helpers

static BOOL IsNumber(CString const& csText)
{
	if (csText.IsEmpty())
	{
		return FALSE;
	}
	for (int i = 0; i < csText.GetLength(); ++i)
	{
		if (!isdigit(csText[i]))
		{
			return FALSE;
		}
	}
	return TRUE;
}

// CNumberEdit

IMPLEMENT_DYNAMIC(CNumberEdit, CEdit)

CNumberEdit::CNumberEdit()
:	m_nMin (-1), m_nMax(-1)
{

}

CNumberEdit::~CNumberEdit()
{
}

void CNumberEdit::SetRange( int nMin, int nMax )
{
	if (nMax > nMin)
	{
		m_nMin = nMin;
		m_nMax = nMax;
	}
}

BEGIN_MESSAGE_MAP(CNumberEdit, CEdit)
	ON_WM_CHAR()
	ON_MESSAGE(WM_PASTE, OnPaste)
END_MESSAGE_MAP()



// CNumberEdit message handlers



void CNumberEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	ProcessTextChangingMessage();
}

LRESULT CNumberEdit::OnPaste(WPARAM wParam, LPARAM lParam)
{
	ProcessTextChangingMessage();
	return 0;
}

BOOL CNumberEdit::IsInRange( int nValue )
{
	if ((m_nMin == -1 || nValue >= m_nMin) && (m_nMax == -1 || nValue <= m_nMax))
	{
		return TRUE;
	}
	return FALSE;
}

void CNumberEdit::ProcessTextChangingMessage()
{
	CString csOrigText;
	GetWindowText(csOrigText);

	Default();

	CString csNewText;
	GetWindowText(csNewText);
	if (!IsNumber(csNewText) || !IsInRange(_tstoi(csNewText)))
	{
		MessageBeep(-1);
		SetWindowText(csOrigText);
	}
}