#pragma once

#include "AviDatesDlg.h"

// CEditPositionDialog dialog

class CEditPositionDialog : public CDialog
{
	DECLARE_DYNAMIC(CEditPositionDialog)

public:
	CEditPositionDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditPositionDialog();
	void SetPosition(int nPos);
	int GetPosition() const;

// Dialog Data
	enum { IDD = IDD_EDIT_POS };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bMoveNextPos;

private:
	int m_pos;
	afx_msg void OnBnClickedOk();
};
