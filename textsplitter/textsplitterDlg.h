// textsplitterDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include <vector>

/**/
struct SplitParams
{
	SplitParams() : 
		nParts(0), nHeaderLines(0), 
		pParentWnd(NULL), pbCanceled(NULL)
	{
		vEOLMarker.push_back('\r');
		vEOLMarker.push_back('\n');
	}

	CString csFileName;
	int nParts;
	int nHeaderLines;
	CWnd* pParentWnd;
	LPBOOL pbCanceled;
	std::vector<byte> vEOLMarker;
};

// CtextsplitterDlg dialog
class CTextSplitterDlg : public CDialog
{
// Construction
public:
	CTextSplitterDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TEXTSPLITTER_DIALOG };

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
	afx_msg void OnBnClickedSplit();
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
	CStatic m_txtProgressCaption;
	CProgressCtrl m_ctrlProgress;
	CSpinButtonCtrl m_spinParts;
	CSpinButtonCtrl m_spinHeaderLines;
	CWinThread* m_pThread;
	BOOL m_bCanceled;
	CButton m_btnSplit;
	SplitParams m_SplitParams;

};
