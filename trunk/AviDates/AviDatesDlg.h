// AviDatesDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "vector"

typedef std::pair<CTimeSpan, int> CPositionInMovie;

CString ToString(CPositionInMovie pos);


// CAviDatesDlg dialog
class CAviDatesDlg : public CDialog
{
// Construction
public:

	CAviDatesDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_AVIDATES_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CListCtrl m_ctrlFiles;
	CListCtrl m_ctrlDates;
	BOOL m_bSortOrder;

	// Generated message map functions
	virtual BOOL OnInitDialog();

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedRemove();
	afx_msg void OnBnClickedUp();
	afx_msg void OnBnClickedDown();
	afx_msg void OnBnClickedCalculate();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedEdit();

	DECLARE_MESSAGE_MAP()

	std::vector<int> GetSelectedFiles();
	void MoveFile(int nIndex, int nStep);
	void InsertFile(int nIndex, CString sName, CString sPath);
	void OnLvnColumnclickFiles(NMHDR *pNMHDR, LRESULT *pResult);
	void OnUpdateControls(NMHDR *pNMHDR, LRESULT *pResult);
	void OnDblClick(NMHDR *pNMHDR, LRESULT *pResult);
	void DeleteFile( int nIndex );
	void OnClose();
	CPositionInMovie FramesToTime(int nFrames);
	void InsertDate(CTime dt, int nFrames);
	void UpdateControls();
	void DeleteAllDates();
};
