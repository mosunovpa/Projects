// TextMergerDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "vector"
#include "afxwin.h"

struct MergeParams
{
	MergeParams() : 
		nHeaderLines(0), 
		pParentWnd(NULL), pbCanceled(NULL)
	{
		vEOLMarker.push_back('\r');
		vEOLMarker.push_back('\n');
	}

	CString csFileName;
	int nHeaderLines;
	CWnd* pParentWnd;
	LPBOOL pbCanceled;
	std::vector<byte> vEOLMarker;
	CStringList slFiles;
};

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

	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual void OnOK() {}

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose( );

	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedRemove();
	afx_msg void OnBnClickedUp();
	afx_msg void OnBnClickedDown();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedMerge();
	afx_msg void OnBnClickedBrowse();
	afx_msg void OnLvnColumnclickFiles(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnProgress(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFinish(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

	std::vector<int> GetSelectedFiles();
	void MoveFile(int nIndex, int nStep);
	void InsertFile(int nIndex, CString sName, CString sPath);
	void DeleteFile(int nIndex);

private:
	BOOL IsRunning();

	HICON m_hIcon;
	HICON m_hIconSm;
	CListCtrl m_ctrlFiles;
	CSpinButtonCtrl m_ctrlHeaderLines;
	CEdit m_ctrlOutputFile;
	CProgressCtrl m_ctrlProgress;
	CButton m_btnMerge;
	BOOL m_bSortOrder;
	CWinThread* m_pThread;
	BOOL m_bCanceled;
	MergeParams m_MergeParams;
	CStatic m_txtProgressCaption;
};
