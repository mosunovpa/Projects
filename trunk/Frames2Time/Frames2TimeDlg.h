// Frames2TimeDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "afxcmn.h"
#include "NumberEdit.h"


// CFrames2TimeDlg dialog
class CFrames2TimeDlg : public CDialog
{
// Construction
public:
	CFrames2TimeDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_FRAMES2TIME_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual void OnOK() {}
	virtual void OnCancel() {}

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedClose();
	afx_msg void OnDtnDatetimechangeTime(NMHDR *pNMHDR, LRESULT *pResult);

	void SetFrames();
	afx_msg void OnEnChangeEditFrames();
	afx_msg void OnEnChangeEditTimeFrames();
	afx_msg void OnClose();

	DECLARE_MESSAGE_MAP()

private:
	CNumberEdit m_ctrlFrames;
	CDateTimeCtrl m_ctrlTime;
	CNumberEdit m_ctrlTimeFrames;
	CSpinButtonCtrl m_ctrlTimeFramesPos;
};
