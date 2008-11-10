// TextMergerDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "vector"

// CTextMergerDlg dialog
class CTextMergerDlg : public CDialog
{
// Construction
public:
	CTextMergerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TEXTMERGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CListCtrl m_ctrlFiles;
	BOOL m_bSortOrder;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy( );

	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedRemove();
	afx_msg void OnBnClickedUp();
	afx_msg void OnBnClickedDown();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedMerge();

	DECLARE_MESSAGE_MAP()

	std::vector<int> GetSelectedFiles();
	void MoveFile(int nIndex, int nStep);
	void InsertFile(int nIndex, CString sName, CString sPath);
	void DeleteFile(int nIndex);
public:
	afx_msg void OnLvnColumnclickFiles(NMHDR *pNMHDR, LRESULT *pResult);
};
