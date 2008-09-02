// textsplitterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "textsplitter.h"
#include "textsplitterDlg.h"
#include "shlwapi.h"
#include <math.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const UINT WMS_START = WM_USER + 101;
const UINT WMS_PROGRESS = WM_USER + 102;
const UINT WMS_FINISH = WM_USER + 103;

/**/
CString GetPartFileName(CString csSplitFileName, int nCurrPart)
{
	LPCTSTR szExt = ::PathFindExtension(csSplitFileName);
	CString csFile(csSplitFileName, (int)(szExt - csSplitFileName));
	CString csFilePart;
	csFilePart.Format(_T("%s_%d%s"), csFile, nCurrPart, szExt);
	return csFilePart;
}

UINT __cdecl Split_Thread( LPVOID pParam )
{
	SplitParams* pSplitParam = (SplitParams*)pParam;
	if (pSplitParam->csFileName.Trim().GetLength() == 0)
	{
		AfxMessageBox(_T("Specify file name."));
		return 0;
	}
	if (!::PathFileExists(pSplitParam->csFileName))
	{
		AfxMessageBox(_T("File is not found."));
		return 0;
	}
	if (pSplitParam->nParts <= 0)
	{
		AfxMessageBox(_T("Specify parts count."));
		return 0;
	}
	
	int nCurrPart = 1;
	CStdioFile fileSplit(pSplitParam->csFileName, CFile::typeText | CFile::modeRead);
	double nFileSize =  (double)fileSplit.GetLength();
	long nPartSize = (long)floor(nFileSize / pSplitParam->nParts + 0.5);
	CString sLine;
	CStdioFile filePart;
	int nReaded = 0;
	int nProgress = 0;

	pSplitParam->pParentWnd->PostMessage(WMS_START, (WPARAM)nFileSize);


	CString sHeader;
	if (pSplitParam->nHeaderLines > 0)
	{
		int n = pSplitParam->nHeaderLines;
		CString sHeaderLine;
		while (n && fileSplit.ReadString(sHeaderLine))
		{
			sHeader += sHeaderLine + _T("\n");
			--n;
		}
	}
	
	filePart.Open(GetPartFileName(pSplitParam->csFileName, nCurrPart), CFile::modeWrite | CFile::modeCreate);
	if (!sHeader.IsEmpty())
	{
		filePart.WriteString(sHeader);
	}

	while (fileSplit.ReadString(sLine))
	{
		if (*(pSplitParam->pbRunning) == FALSE)
		{
			break;
		}
		filePart.WriteString(sLine + _T("\n"));
		nReaded += sLine.GetLength() + 1;
		nProgress += sLine.GetLength() + 1;

		pSplitParam->pParentWnd->PostMessage(WMS_PROGRESS, (WPARAM)nProgress, (LPARAM)nFileSize);

		if (nReaded >= nPartSize)
		{
			nReaded = (nReaded % nPartSize);
			filePart.Close();
			filePart.Open(GetPartFileName(pSplitParam->csFileName, ++nCurrPart), CFile::modeWrite | CFile::modeCreate);
			if (!sHeader.IsEmpty())
			{
				filePart.WriteString(sHeader);
			}
		}
	}
	filePart.Close();
	fileSplit.Close();

	pSplitParam->pParentWnd->PostMessage(WMS_FINISH);

	return 0;
}


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CtextsplitterDlg dialog




CtextsplitterDlg::CtextsplitterDlg(CWnd* pParent /*=NULL*/)
	:	CDialog(CtextsplitterDlg::IDD, pParent), 
		m_pThread(NULL),
		m_bRunning(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtextsplitterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN_PARTS, m_spinParts);
	DDX_Control(pDX, IDC_SPIN_HEADER, m_spinHeaderLines);
	DDX_Control(pDX, IDC_EDIT_FILE, m_editFile);
	DDX_Control(pDX, IDC_PROGRESS, m_ctrlProgress);
	DDX_Control(pDX, IDSPLIT, m_btnSplit);
}

BEGIN_MESSAGE_MAP(CtextsplitterDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDSPLIT, &CtextsplitterDlg::OnBnClickedSplit)
	ON_BN_CLICKED(IDCLOSE, &CtextsplitterDlg::OnBnClickedClose)
	ON_BN_CLICKED(IDC_FILE, &CtextsplitterDlg::OnBnClickedFile)

	ON_MESSAGE(WMS_START, OnStart)
	ON_MESSAGE(WMS_PROGRESS, OnProgress)
	ON_MESSAGE(WMS_FINISH, OnFinish)

END_MESSAGE_MAP()


// CtextsplitterDlg message handlers

BOOL CtextsplitterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_spinParts.SetRange(0, 100);
	m_spinHeaderLines.SetRange(0, 100);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CtextsplitterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CtextsplitterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CtextsplitterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/* */
void CtextsplitterDlg::OnCancel()
{
	// do nothing
}

/**/
void CtextsplitterDlg::OnClose()
{
	StopThread();
	delete m_pThread;

	EndDialog(IDCANCEL);
}

/**/
void CtextsplitterDlg::OnBnClickedSplit()
{
	if (m_bRunning)
	{
		StopThread();
	}
	else
	{
		CString csFileName;
		m_editFile.GetWindowText(csFileName);
		m_SplitParams.csFileName = csFileName;
		m_SplitParams.nParts = m_spinParts.GetPos();
		m_SplitParams.nHeaderLines = m_spinHeaderLines.GetPos();
		m_SplitParams.pParentWnd = this;
		m_SplitParams.pbRunning = &m_bRunning;
		if (StopThread())
		{
			m_bRunning = TRUE;
			m_pThread = AfxBeginThread(Split_Thread, &m_SplitParams, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
			m_pThread->m_bAutoDelete = FALSE;
			m_pThread->ResumeThread();
		}
	}
}

/**/
BOOL CtextsplitterDlg::StopThread()
{
	m_bRunning = FALSE;
	if (m_pThread != NULL)
	{
		if (WAIT_OBJECT_0 != WaitForSingleObject(m_pThread->m_hThread, 60000))
		{
			AfxMessageBox(_T("Error thread stopping"));
			return FALSE;
		}
	}

	delete m_pThread;
	m_pThread = NULL;

	return TRUE;
}

/**/
void CtextsplitterDlg::OnBnClickedClose()
{
	PostMessage(WM_CLOSE);
}

/**/
void CtextsplitterDlg::OnBnClickedFile()
{
	CString csFileName;
	m_editFile.GetWindowText(csFileName);
	CFileDialog fdlg(TRUE, _T("*.csv"), csFileName, OFN_FILEMUSTEXIST, 
		_T("CSV Files (*.csv)|*.csv|All Files|*.*||"));
	if (fdlg.DoModal())
	{
		m_editFile.SetWindowText(fdlg.GetPathName());
	}
}

LRESULT CtextsplitterDlg::OnStart( WPARAM wParam, LPARAM lParam )
{
	m_ctrlProgress.SetRange32(0, (int)wParam);
	m_ctrlProgress.SetPos(0);
	m_btnSplit.SetWindowText(_T("Cancel"));
	return 0;
}

LRESULT CtextsplitterDlg::OnProgress( WPARAM wParam, LPARAM lParam )
{
	m_ctrlProgress.SetPos((int)wParam);
	return 0;
}

LRESULT CtextsplitterDlg::OnFinish( WPARAM wParam, LPARAM lParam )
{
	StopThread();
	m_ctrlProgress.SetPos(0);
	m_btnSplit.SetWindowText(_T("Split"));
	return 0;
}