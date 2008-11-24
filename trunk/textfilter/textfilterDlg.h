// textfilterDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include <vector>

/**/
struct FilterParams
{
	FilterParams() : 
		nPos(0), nHeader(0),
		pParentWnd(NULL), pbCanceled(NULL)
	{
	}

	CString csFileName;
	int nPos;
	int nHeader;
	CString csFilter;
	CWnd* pParentWnd;
	LPBOOL pbCanceled;
};

// CTextFilterDlg dialog
class CTextFilterDlg : public CDialog
{
// Construction
public:
	CTextFilterDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TEXTFILTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	HICON m_hIconSm;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnBnClickedFilter();
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedFile();

	afx_msg LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnProgress(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFinish(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()


private:

	BOOL StopThread();
	BOOL IsRunning();

	CEdit m_editFile;
	CEdit m_editFilter;
	CStatic m_txtProgressCaption;
	CProgressCtrl m_ctrlProgress;
	CSpinButtonCtrl m_spinPos;
	CWinThread* m_pThread;
	BOOL m_bCanceled;
	CButton m_btnSplit;
	FilterParams m_FilterParams;
	CSpinButtonCtrl m_ctrlHeaderLines;
};
