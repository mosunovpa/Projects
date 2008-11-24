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
CString GetOutFileName(CString csSplitFileName)
{
	LPCTSTR szExt = ::PathFindExtension(csSplitFileName);
	CString csFile(csSplitFileName, (int)(szExt - csSplitFileName));
	CString csFilePart;
	csFilePart.Format(_T("%s_filtered%s"), csFile, szExt);
	int cnt = 1;
	while (::PathFileExists(csFilePart))
	{
		csFilePart.Format(_T("%s_filtered(%d)%s"), csFile, cnt, szExt);
		++cnt;
	}
	return csFilePart;
}

BOOL Consist(LPCTSTR pszLine, int nPos, CStringList const& filter)
{
	CString csLine(pszLine);
	int nStart = 0;
	int nEnd = -1;
	// go to needed position
	while (nPos)
	{
		nStart = nEnd + 1;
		nEnd = csLine.Find(_T(";"), nStart);
		if (nEnd == -1)
		{
			if (nPos > 1) // too big position
			{
				return FALSE;
			}
			nEnd = csLine.GetLength();
			break;
		}
		--nPos;
	}

	CString csField((LPCTSTR)csLine + nStart, nEnd - nStart);
	POSITION pos = filter.GetHeadPosition();
	while (pos)
	{
		CString const& csFilter = filter.GetNext(pos);
		if (!csFilter.IsEmpty() && csField.Find(csFilter) != -1)
		{
			return TRUE;
		}
	}
	return FALSE;
}

 
void ParseFilter(CString csFilter, CStringList& OUT filters)
{
	int pos = 0;
	int nStart = 0;
	CString csLex;
	while ((pos = csFilter.Find(_T(";"), nStart)) != -1)
	{
		csLex = CString(((LPCTSTR)csFilter) + nStart, pos - nStart);
		if (!csLex.IsEmpty())
		{
			filters.AddTail(csLex);
		}
		nStart = pos + 1;
	}
	csLex = CString(((LPCTSTR)csFilter) + nStart, csFilter.GetLength() - nStart);
	if (!csLex.IsEmpty())
	{
		filters.AddTail(csLex);
	}
}


UINT __cdecl Filter_Thread( LPVOID pParam )
{
	FilterParams* pFilterParam = (FilterParams*)pParam;
	if (pFilterParam->csFileName.Trim().GetLength() == 0)
	{
		AfxMessageBox(_T("Specify file name."));
		return 0;
	}
	if (!::PathFileExists(pFilterParam->csFileName))
	{
		AfxMessageBox(_T("File is not found."));
		return 0;
	}
	if (pFilterParam->csFilter.IsEmpty())
	{
		AfxMessageBox(_T("Filter is not defined."));
		return 0;
	}
	
	CStdioFile fileInput(pFilterParam->csFileName, CFile::modeRead);

	int nReaded = 0;

	ULONGLONG nFileSize = fileInput.GetLength();
	int nProgress = 0;
	int nProgressPartSize = (int)(nFileSize / 50);
	int nProgressPart = 0;

	pFilterParam->pParentWnd->PostMessage(WMS_START, (WPARAM)nFileSize);
	
	CStdioFile fileOutput(GetOutFileName(pFilterParam->csFileName), CFile::modeWrite | CFile::modeCreate);
	TCHAR line[4096];

	// skip header
	CStringList slistHeader;
	while (pFilterParam->nHeader)
	{
		fileInput.ReadString(line, 4096);
		slistHeader.AddTail(line);
		--pFilterParam->nHeader;
	}
	// copy header
	POSITION pos = slistHeader.GetHeadPosition();
	while (pos)
	{
		CString const& csHeaderLine = slistHeader.GetNext(pos);
		fileOutput.WriteString(csHeaderLine);
	}

	// copy content
	CStringList filters; 
	ParseFilter(pFilterParam->csFilter, filters);
	while (fileInput.ReadString(line, 2048))
	{
		if (*(pFilterParam->pbCanceled) == TRUE)
		{
			break;
		}			
		if (Consist(line, pFilterParam->nPos, filters))
		{
			fileOutput.WriteString(line);
		}
		// update progress bar
		nReaded = lstrlen(line);
		nProgress += nReaded;
		nProgressPart += nReaded;
		if (nProgressPart > nProgressPartSize)
		{
			nProgressPart = 0;
			pFilterParam->pParentWnd->PostMessage(WMS_PROGRESS, (WPARAM)nProgress, (LPARAM)nFileSize);
		}
	}
	pFilterParam->pParentWnd->PostMessage(WMS_FINISH, *pFilterParam->pbCanceled);

	fileOutput.Close();
	fileInput.Close();

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
	DDX_Control(pDX, IDC_SPIN_POS, m_spinPos);
	DDX_Control(pDX, IDC_EDIT_FILE, m_editFile);
	DDX_Control(pDX, IDC_EDIT_FILTER, m_editFilter);
	DDX_Control(pDX, IDC_PROGRESS, m_ctrlProgress);
	DDX_Control(pDX, IDFILTER, m_btnSplit);
	DDX_Control(pDX, IDC_STATIC_PROGRESS, m_txtProgressCaption);
	DDX_Control(pDX, IDC_SPIN_HEADER, m_ctrlHeaderLines);
}

BEGIN_MESSAGE_MAP(CTextFilterDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDFILTER, &CTextFilterDlg::OnBnClickedFilter)
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
	m_spinPos.SetRange(1, 100);
	m_spinPos.SetPos(1);
	m_ctrlHeaderLines.SetRange(1, 100);
	m_ctrlHeaderLines.SetPos(1);

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
void CTextFilterDlg::OnBnClickedFilter()
{
	if (IsRunning())
	{
		::InterlockedExchange((LONG*)&m_bCanceled, TRUE);
	}
	else
	{
		m_editFile.GetWindowText(m_FilterParams.csFileName);
		m_FilterParams.nPos = m_spinPos.GetPos();
		m_FilterParams.nHeader = m_ctrlHeaderLines.GetPos();
		m_editFilter.GetWindowText(m_FilterParams.csFilter);
		m_FilterParams.pParentWnd = this;
		m_FilterParams.pbCanceled = &m_bCanceled;

		m_bCanceled = FALSE;
		delete m_pThread;
		m_pThread = AfxBeginThread(Filter_Thread, &m_FilterParams, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
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
	if (fdlg.DoModal() == IDOK)
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
	m_btnSplit.SetWindowText(_T("Filter"));
	m_txtProgressCaption.ShowWindow(SW_HIDE);
	m_ctrlProgress.ShowWindow(SW_HIDE);
	if (wParam == 0) // successfully
	{
		AfxMessageBox(_T("File has been filtered successfully."));
	}
	else // canceled
	{
		AfxMessageBox(_T("Filtering canceled."));
	}

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
