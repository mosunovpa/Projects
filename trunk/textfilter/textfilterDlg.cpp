// textfilter.Dlg.cpp : implementation file
//

#include "stdafx.h"
#include <math.h>
#include "textfilter.h"
#include "textfilterDlg.h"
#include "shlwapi.h"


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

/**/
BOOL FindEOL(std::vector<byte>buf, int nStart, int nEnd, std::vector<byte> vEOLMarker, int& nEOLPos, int& nCurMarkSymb)
{
	// найти строку
	for (nEOLPos = nStart; nEOLPos < nEnd; ++nEOLPos)
	{
		if (buf[nEOLPos] == vEOLMarker[nCurMarkSymb])
		{
			++nCurMarkSymb;
			if (nCurMarkSymb == vEOLMarker.size()) // found
			{
				nCurMarkSymb = 0;
				++nEOLPos; // set next position
				return TRUE;
			}
		}
		else
		{
			nCurMarkSymb = 0;
		}
	}
	return FALSE;
}

UINT __cdecl Split_Thread( LPVOID pParam )
{
	SearchParams* pSplitParam = (SearchParams*)pParam;
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
	CFile fileSplit(pSplitParam->csFileName, CFile::modeRead);

	std::vector<byte> buf(128);

	int nHeadLinesLeft = pSplitParam->nHeaderLines;
	int nCurMarkSymb = 0; // current character in line marker 
	int nEOLPos;	// end line position
	std::vector<byte> vHeader;
	std::back_insert_iterator< std::vector<byte> > itHeader(vHeader);
	int nReaded = 0;

	// read header
	while (nHeadLinesLeft && (nReaded = fileSplit.Read(&buf[0], (int)buf.size())))
	{
		int nStart = 0;
		while (nHeadLinesLeft && FindEOL(buf, nStart, nReaded, pSplitParam->vEOLMarker, nEOLPos, nCurMarkSymb))
		{
			std::copy(buf.begin() + nStart, buf.begin() + nEOLPos, itHeader);
			nStart = nEOLPos;
			--nHeadLinesLeft;
		}
		if (nHeadLinesLeft)
		{
			std::copy(buf.begin(), buf.end(), itHeader);
		}
	}
	fileSplit.Seek(vHeader.size(), CFile::begin);

	ULONGLONG nFileSize = fileSplit.GetLength() - vHeader.size();
	int nPartSize = (int)(nFileSize / pSplitParam->nParts);
	CFile filePart;
	int nReadedInPart = 0;
	int nProgress = 0;
	int nProgressPartSize = (int)(nFileSize / 50);
	int nProgressPart = 0;

	pSplitParam->pParentWnd->PostMessage(WMS_START, (WPARAM)nFileSize);
	
	filePart.Open(GetPartFileName(pSplitParam->csFileName, nCurrPart), CFile::modeWrite | CFile::modeCreate);
	if (!vHeader.empty())
	{
		filePart.Write(&vHeader[0], (int)vHeader.size());
	}
	
	int nBufSize = min(nPartSize, (int)buf.size());
	while (nReaded = fileSplit.Read(&buf[0], nBufSize))
	{
		if (*(pSplitParam->pbCanceled) == TRUE)
		{
			break;
		}	
		if (nReadedInPart >= nPartSize &&
			FindEOL(buf, 0, nReaded, pSplitParam->vEOLMarker, nEOLPos, nCurMarkSymb)
			) 
		{
			// write line and start next part
			nReadedInPart = 0;
			nProgress += nEOLPos;
			fileSplit.Seek(vHeader.size() + nProgress, CFile::begin);

			filePart.Write(&buf[0], nEOLPos);
			filePart.Close();
			filePart.Open(GetPartFileName(pSplitParam->csFileName, ++nCurrPart), CFile::modeWrite | CFile::modeCreate);
			if (!vHeader.empty())
			{
				filePart.Write(&vHeader[0], (int)vHeader.size());
			}
		}
		else
		{
			filePart.Write(&buf[0], nReaded);
			nReadedInPart += nReaded;
			nProgress += nReaded;
		}

		// update progress bar
		nProgressPart += nReaded;
		if (nProgressPart > nProgressPartSize)
		{
			nProgressPart = 0;
			pSplitParam->pParentWnd->PostMessage(WMS_PROGRESS, (WPARAM)nProgress, (LPARAM)nFileSize);
		}
	}
	pSplitParam->pParentWnd->PostMessage(WMS_FINISH);

	filePart.Close();
	fileSplit.Close();

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


// CTextFilterDlg dialog




CTextFilterDlg::CTextFilterDlg(CWnd* pParent /*=NULL*/)
	:	CDialog(CTextFilterDlg::IDD, pParent), 
		m_pThread(NULL),
		m_bCanceled(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hIconSm = AfxGetApp()->LoadIcon(IDI_SMALL);
}

void CTextFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN_PARTS, m_spinParts);
	DDX_Control(pDX, IDC_SPIN_HEADER, m_spinHeaderLines);
	DDX_Control(pDX, IDC_EDIT_FILE, m_editFile);
	DDX_Control(pDX, IDC_PROGRESS, m_ctrlProgress);
	DDX_Control(pDX, IDSPLIT, m_btnSplit);
	DDX_Control(pDX, IDC_STATIC_PROGRESS, m_txtProgressCaption);
}

BEGIN_MESSAGE_MAP(CTextFilterDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDSPLIT, &CTextFilterDlg::OnBnClickedSplit)
	ON_BN_CLICKED(IDCLOSE, &CTextFilterDlg::OnBnClickedClose)
	ON_BN_CLICKED(IDC_FILE, &CTextFilterDlg::OnBnClickedFile)

	ON_MESSAGE(WMS_START, OnStart)
	ON_MESSAGE(WMS_PROGRESS, OnProgress)
	ON_MESSAGE(WMS_FINISH, OnFinish)

END_MESSAGE_MAP()


// CTextFilterDlg message handlers

BOOL CTextFilterDlg::OnInitDialog()
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
	SetIcon(m_hIconSm, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_spinParts.SetRange(0, 100);
	m_spinHeaderLines.SetRange(0, 100);
	m_spinParts.SetPos(2);
	m_spinHeaderLines.SetPos(1);

	m_txtProgressCaption.ShowWindow(SW_HIDE);
	m_ctrlProgress.ShowWindow(SW_HIDE);


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTextFilterDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTextFilterDlg::OnPaint()
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
		dc.DrawIcon(x, y, m_hIconSm);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTextFilterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/* */
void CTextFilterDlg::OnCancel()
{
	// do nothing
}

/**/
void CTextFilterDlg::OnClose()
{
	StopThread();
	delete m_pThread;

	EndDialog(IDCANCEL);
}

/**/
void CTextFilterDlg::OnBnClickedSplit()
{
	if (IsRunning())
	{
		::InterlockedExchange((LONG*)&m_bCanceled, TRUE);
	}
	else
	{
		CString csFileName;
		m_editFile.GetWindowText(csFileName);
		m_SplitParams.csFileName = csFileName;
		m_SplitParams.nParts = m_spinParts.GetPos();
		m_SplitParams.nHeaderLines = m_spinHeaderLines.GetPos();
		m_SplitParams.pParentWnd = this;
		m_SplitParams.pbCanceled = &m_bCanceled;

		m_bCanceled = FALSE;
		delete m_pThread;
		m_pThread = AfxBeginThread(Split_Thread, &m_SplitParams, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		m_pThread->m_bAutoDelete = FALSE;
		m_pThread->ResumeThread();
	}
}

/**/
BOOL CTextFilterDlg::StopThread()
{
	::InterlockedExchange((LONG*)&m_bCanceled, TRUE);
	if (m_pThread != NULL)
	{
		if (WAIT_OBJECT_0 != WaitForSingleObject(m_pThread->m_hThread, 10000))
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
void CTextFilterDlg::OnBnClickedClose()
{
	PostMessage(WM_CLOSE);
}

/**/
void CTextFilterDlg::OnBnClickedFile()
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

LRESULT CTextFilterDlg::OnStart( WPARAM wParam, LPARAM lParam )
{
	m_ctrlProgress.SetRange32(0, (int)wParam);
	m_ctrlProgress.SetPos(0);
	m_btnSplit.SetWindowText(_T("Cancel"));
	m_txtProgressCaption.ShowWindow(SW_SHOW);
	m_ctrlProgress.ShowWindow(SW_SHOW);

	return 0;
}

LRESULT CTextFilterDlg::OnProgress( WPARAM wParam, LPARAM lParam )
{
	m_ctrlProgress.SetPos((int)wParam);
	return 0;
}

LRESULT CTextFilterDlg::OnFinish( WPARAM wParam, LPARAM lParam )
{
	m_bCanceled = FALSE;
	m_ctrlProgress.SetPos(0);
	m_btnSplit.SetWindowText(_T("Split"));
	m_txtProgressCaption.ShowWindow(SW_HIDE);
	m_ctrlProgress.ShowWindow(SW_HIDE);
	AfxMessageBox(_T("File has been splitted successfully"));

	return 0;
}

BOOL CTextFilterDlg::IsRunning()
{
	if (m_pThread != NULL)
	{
		DWORD nExitCode;
		if (GetExitCodeThread(m_pThread->m_hThread, &nExitCode)
			&& nExitCode == STILL_ACTIVE)
		{
			return TRUE;
		}
	}
	return FALSE;
}