// TextMergerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TextMerger.h"
#include "TextMergerDlg.h"
#include "shlwapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// helpers

const UINT WMS_START = WM_USER + 101;
const UINT WMS_PROGRESS = WM_USER + 102;
const UINT WMS_FINISH = WM_USER + 103;

std::vector<CString> GetFiles()
{
	std::vector<CString> files;
	CFileDialog dlgFiles(TRUE, _T("csv"), NULL, 
		OFN_ENABLESIZING | OFN_ALLOWMULTISELECT | OFN_EXPLORER | OFN_FILEMUSTEXIST, 
		_T("CSV files (*.csv)|*.csv|All Files (*.*)|*.*||"));
	CString fileName;
	dlgFiles.GetOFN().lpstrFile = fileName.GetBufferSetLength(1000 * (_MAX_PATH + 1) + 1);
	dlgFiles.GetOFN().nMaxFile = fileName.GetLength();
	if (dlgFiles.DoModal() == IDOK)
	{
		LPCTSTR p = (LPCTSTR)fileName;
		int len = lstrlen(p);
		while (len)
		{
			files.push_back(CString(p));
			p += (len + 1);
			len = lstrlen(p);
		}
	}
	fileName.ReleaseBuffer();
	return files;
}

ULONGLONG GetFilesSizes(CStringList const& slFiles)
{
	ULONGLONG result = 0;
	POSITION pos = slFiles.GetHeadPosition();
	while (pos)
	{
		CString const& csFileInput = slFiles.GetNext(pos);
		CFile fileInput(csFileInput, CFile::modeRead);
		result += fileInput.GetLength();
		fileInput.Close();
	}
	return result;
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

UINT __cdecl Merge_Thread( LPVOID pParam )
{
	MergeParams* pMergeParam = (MergeParams*)pParam;
	if (pMergeParam->csFileName.Trim().GetLength() == 0)
	{
		AfxMessageBox(_T("Specify file name."));
		return 0;
	}
	if (pMergeParam->slFiles.GetCount() == 0)
	{
		AfxMessageBox(_T("Specify input files."));
		return 0;
	}
	CFile fileOutput;
	if (!fileOutput.Open(pMergeParam->csFileName, CFile::modeCreate | CFile::modeWrite))
	{
		AfxMessageBox(_T("Can not open output file."));
		return 0;
	}

	ULONGLONG nFileSize = GetFilesSizes(pMergeParam->slFiles);
	pMergeParam->pParentWnd->PostMessage(WMS_START, (WPARAM)nFileSize);

	std::vector<byte> buf(128);

	int nHeadLinesLeft = pMergeParam->nHeaderLines;
	int nCurMarkSymb = 0; // current character in line marker 
	int nEOLPos;	// end line position
	std::vector<byte> vHeader;
	std::back_insert_iterator< std::vector<byte> > itHeader(vHeader);
	int nReaded = 0;


	// merge files
	int nProgress = 0;
	int nProgressPart = 0;
	int nProgressPartSize = (int)(nFileSize / 50);

	POSITION pos = pMergeParam->slFiles.GetHeadPosition();
	while (pos)
	{
		CString& csFileInput = pMergeParam->slFiles.GetNext(pos);
		CFile fileInput(csFileInput, CFile::modeRead);

		// read header from first input file
		if (nHeadLinesLeft)
		{
			// read header
			while (nHeadLinesLeft && (nReaded = fileInput.Read(&buf[0], (int)buf.size())))
			{
				int nStart = 0;
				while (nHeadLinesLeft && FindEOL(buf, nStart, nReaded, pMergeParam->vEOLMarker, nEOLPos, nCurMarkSymb))
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
			if (!vHeader.empty())
			{
				fileOutput.Write(&vHeader[0], (int)vHeader.size());
			}
		}

		if (!vHeader.empty())
		{
			fileInput.Seek(vHeader.size(), CFile::begin);
		}

		while (nReaded = fileInput.Read(&buf[0], (int)buf.size()))
		{
			if (*(pMergeParam->pbCanceled) == TRUE)
			{
				break;
			}	
			fileOutput.Write(&buf[0], nReaded);

			nProgress += nReaded;
			nProgressPart += nReaded;
			if (nProgressPart > nProgressPartSize)
			{
				nProgressPart = 0;
				pMergeParam->pParentWnd->PostMessage(WMS_PROGRESS, (WPARAM)nProgress, (LPARAM)nFileSize);
			}
		}
		fileInput.Close();
	}
	fileOutput.Close();

	pMergeParam->pParentWnd->PostMessage(WMS_FINISH, *(pMergeParam->pbCanceled));

	return 0;
}

//////////////////////////////////////////////////////////////////////////
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


//////////////////////////////////////////////////////////////////////////
// CTextMergerDlg dialog




CTextMergerDlg::CTextMergerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTextMergerDlg::IDD, pParent)
	, m_bSortOrder(FALSE)
	, m_pThread(NULL)
	, m_bCanceled(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hIconSm = AfxGetApp()->LoadIcon(IDR_MAINFRAME_SM);

}

void CTextMergerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILES, m_ctrlFiles);
	DDX_Control(pDX, IDC_SPIN_HEADER, m_ctrlHeaderLines);
	DDX_Control(pDX, IDC_EDIT_OUTPUT_FILE, m_ctrlOutputFile);
	DDX_Control(pDX, IDC_PROGRESS, m_ctrlProgress);
	DDX_Control(pDX, IDC_MERGE, m_btnMerge);
	DDX_Control(pDX, IDC_STATIC_PROGRESS, m_txtProgressCaption);
}

BEGIN_MESSAGE_MAP(CTextMergerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_ADD, &CTextMergerDlg::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_REMOVE, &CTextMergerDlg::OnBnClickedRemove)
	ON_BN_CLICKED(IDC_UP, &CTextMergerDlg::OnBnClickedUp)
	ON_BN_CLICKED(IDC_DOWN, &CTextMergerDlg::OnBnClickedDown)
	ON_BN_CLICKED(IDCANCEL, &CTextMergerDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_MERGE, &CTextMergerDlg::OnBnClickedMerge)
	ON_BN_CLICKED(IDC_BTN_BROWSE_OUTPUT, &CTextMergerDlg::OnBnClickedBrowse)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_FILES, &CTextMergerDlg::OnLvnColumnclickFiles)

	ON_MESSAGE(WMS_START, OnStart)
	ON_MESSAGE(WMS_PROGRESS, OnProgress)
	ON_MESSAGE(WMS_FINISH, OnFinish)

END_MESSAGE_MAP()


// CTextMergerDlg message handlers

BOOL CTextMergerDlg::OnInitDialog()
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

//	m_ctrlFiles.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_ctrlFiles.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 150);
	m_ctrlFiles.InsertColumn(1, _T("Path"), LVCFMT_LEFT, 230);

	m_ctrlHeaderLines.SetRange(0, 100);
	m_ctrlHeaderLines.SetPos(1);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTextMergerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTextMergerDlg::OnPaint()
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
HCURSOR CTextMergerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIconSm);
}

void CTextMergerDlg::OnBnClickedAdd()
{
	std::vector<CString> files = GetFiles();
	CString csPath;
	CString csName;
	if (files.size() == 1)
	{
		LPTSTR sName = ::PathFindFileName(files[0]);
		csPath = CString(files[0], sName - ((LPCTSTR)files[0]) - 1);
		InsertFile(m_ctrlFiles.GetItemCount(), sName, csPath);
		csName = sName;
	}
	else if (files.size() > 1)
	{
		for (int i = 1; i < (int)files.size(); ++i)
		{
			InsertFile(m_ctrlFiles.GetItemCount(), files[i], files[0]);
		}
		csPath = files[0];
		csName = files[1];
	}

	CString csOutFileName;
	m_ctrlOutputFile.GetWindowText(csOutFileName);
	if (!csPath.IsEmpty() && csOutFileName.Trim().IsEmpty())
	{
		csOutFileName = csPath;
		csOutFileName += _T("\\");
		LPCTSTR sName = (LPCTSTR)csName;
		LPCTSTR sExt = ::PathFindExtension(sName);
		csOutFileName += CString(sName, sExt - sName);
		csOutFileName += _T("_merged");
		csOutFileName += sExt;
		m_ctrlOutputFile.SetWindowText(csOutFileName);
	}
}
void CTextMergerDlg::OnClose()
{
	::InterlockedExchange((LONG*)&m_bCanceled, TRUE);
	if (m_pThread != NULL)
	{
		if (WAIT_OBJECT_0 != WaitForSingleObject(m_pThread->m_hThread, 10000))
		{
			AfxMessageBox(_T("Error thread stopping"));
			return;
		}
	}
	delete m_pThread;

	int cnt = m_ctrlFiles.GetItemCount();
	if (cnt > 0)
	{
		for (int i = cnt - 1; i >= 0; --i)
		{
			DeleteFile(i);
		}
	}

	EndDialog(IDCANCEL);
}

void CTextMergerDlg::OnBnClickedRemove()
{
	std::vector<int> vDeleted = GetSelectedFiles();
	if (!vDeleted.empty())
	{
		for (int i = (int)vDeleted.size() - 1; i >= 0; --i)
		{
			DeleteFile(vDeleted[i]);
		}
	}
}

void CTextMergerDlg::OnBnClickedUp()
{
	std::vector<int> vSelected = GetSelectedFiles();
	if (!vSelected.empty())
	{
		for (int i = 0; i < (int)vSelected.size(); ++i)
		{
			MoveFile(vSelected[i], -1);
		}
	}
}

void CTextMergerDlg::OnBnClickedDown()
{
	std::vector<int> vSelected = GetSelectedFiles();
	if (!vSelected.empty())
	{
		for (int i = (int)vSelected.size() - 1; i >= 0; --i)
		{
			MoveFile(vSelected[i], 1);
		}
	}
}

void CTextMergerDlg::OnBnClickedCancel()
{
	OnCancel();
}

std::vector<int> CTextMergerDlg::GetSelectedFiles()
{
	std::vector<int> vSelected;
	POSITION nPos = m_ctrlFiles.GetFirstSelectedItemPosition();
	while (nPos)
	{
		int nIndex = m_ctrlFiles.GetNextSelectedItem(nPos);
		vSelected.push_back(nIndex);
	}
	return vSelected;
}

void CTextMergerDlg::MoveFile(int nIndex, int nStep)
{
	int nTargetIndex = nIndex + nStep;
	if (nIndex >= 0 && nIndex < m_ctrlFiles.GetItemCount() &&
		nTargetIndex >= 0 && nTargetIndex < m_ctrlFiles.GetItemCount() &&
		m_ctrlFiles.GetItemState(nTargetIndex, LVIS_SELECTED) != LVIS_SELECTED)
	{
		CString sName = m_ctrlFiles.GetItemText(nIndex, 0);
		CString sPath = m_ctrlFiles.GetItemText(nIndex, 1);
		DeleteFile(nIndex);
		InsertFile(nTargetIndex, sName, sPath);
		m_ctrlFiles.SetItemState(nTargetIndex, LVIS_SELECTED, LVIS_SELECTED);
	}
}

/**/
void CTextMergerDlg::InsertFile(int nIndex, CString sName, CString sPath)
{
	int nPos = m_ctrlFiles.InsertItem(nIndex, sName);
	m_ctrlFiles.SetItem(nPos, 1, LVIF_TEXT, sPath, 0, 0, 0, 0);
	m_ctrlFiles.SetItemData(nPos, (DWORD_PTR)(new CString(sName)));
}

/**/
void CTextMergerDlg::DeleteFile( int nIndex )
{
	delete (CString*)(m_ctrlFiles.GetItemData(nIndex));
	m_ctrlFiles.DeleteItem(nIndex);
}

void CTextMergerDlg::OnBnClickedMerge()
{
	if (IsRunning())
	{
		::InterlockedExchange((LONG*)&m_bCanceled, TRUE);
	}
	else
	{

		m_MergeParams.slFiles.RemoveAll();
		int cnt = m_ctrlFiles.GetItemCount();
		for (int i = 0; i < cnt; ++i)
		{
			m_MergeParams.slFiles.AddTail(m_ctrlFiles.GetItemText(i, 0));
		}

		m_ctrlOutputFile.GetWindowText(m_MergeParams.csFileName);
		m_MergeParams.nHeaderLines = m_ctrlHeaderLines.GetPos();
		m_MergeParams.pParentWnd = this;
		m_MergeParams.pbCanceled = &m_bCanceled;


		m_bCanceled = FALSE;
		delete m_pThread;
		m_pThread = AfxBeginThread(Merge_Thread, &m_MergeParams, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		m_pThread->m_bAutoDelete = FALSE;
		m_pThread->ResumeThread();
	}

}

// Sort items by associated lParam
static int CALLBACK FilesCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CString* ps1 = (CString*)lParam1;
	CString* ps2 = (CString*)lParam2;

	return lParamSort ? lstrcmp(ps2->GetString(), ps1->GetString()) :
		lstrcmp(ps1->GetString(), ps2->GetString());
}

void CTextMergerDlg::OnLvnColumnclickFiles(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if (pNMLV->iSubItem == 0)
	{
		m_ctrlFiles.SortItems(FilesCompareProc, (DWORD_PTR)m_bSortOrder);
		m_bSortOrder = !m_bSortOrder;
	}
	*pResult = 0;
}

/**/
void CTextMergerDlg::OnBnClickedBrowse()
{
	CFileDialog dlgFiles(FALSE, _T("csv"), NULL, 
		OFN_ENABLESIZING | OFN_EXPLORER | OFN_OVERWRITEPROMPT, 
		_T("CSV files (*.csv)|*.csv|All Files (*.*)|*.*||"));
	if (dlgFiles.DoModal() == IDOK)
	{
		CString fileName = dlgFiles.GetPathName();
		m_ctrlOutputFile.SetWindowText(fileName);
	}
}

BOOL CTextMergerDlg::IsRunning()
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


LRESULT CTextMergerDlg::OnStart( WPARAM wParam, LPARAM lParam )
{
	m_ctrlProgress.SetRange32(0, (int)wParam);
	m_ctrlProgress.SetPos(0);
	m_btnMerge.SetWindowText(_T("Cancel"));
	m_txtProgressCaption.ShowWindow(SW_SHOW);
	m_ctrlProgress.ShowWindow(SW_SHOW);

	return 0;
}

LRESULT CTextMergerDlg::OnProgress( WPARAM wParam, LPARAM lParam )
{
	m_ctrlProgress.SetPos((int)wParam);
	return 0;
}

LRESULT CTextMergerDlg::OnFinish( WPARAM wParam, LPARAM lParam )
{
	m_bCanceled = FALSE;
	m_ctrlProgress.SetPos(0);
	m_btnMerge.SetWindowText(_T("Merge"));
	m_txtProgressCaption.ShowWindow(SW_HIDE);
	m_ctrlProgress.ShowWindow(SW_HIDE);
	if (wParam == 0) // successfully
	{
		AfxMessageBox(_T("Files have been merged successfully."));
	}
	else
	{
		AfxMessageBox(_T("Merging canceled."));
	}

	return 0;
}