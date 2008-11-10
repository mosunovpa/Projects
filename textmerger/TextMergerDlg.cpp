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

std::vector<CString> GetFiles()
{
	std::vector<CString> files;
	CFileDialog dlgFiles(TRUE, _T("dvdt"), NULL, 
		OFN_ENABLESIZING | OFN_ALLOWMULTISELECT | OFN_EXPLORER | OFN_FILEMUSTEXIST, 
		_T("CSV files (*.csv)|*.csv|All Files (*.*)|*.*||"));
	CString fileName;
	dlgFiles.GetOFN().lpstrFile = fileName.GetBuffer(1000 * (_MAX_PATH + 1) + 1);
	dlgFiles.GetOFN().nMaxFile = 1000;
	if (dlgFiles.DoModal())
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
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTextMergerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILES, m_ctrlFiles);
}

BEGIN_MESSAGE_MAP(CTextMergerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_ADD, &CTextMergerDlg::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_REMOVE, &CTextMergerDlg::OnBnClickedRemove)
	ON_BN_CLICKED(IDC_UP, &CTextMergerDlg::OnBnClickedUp)
	ON_BN_CLICKED(IDC_DOWN, &CTextMergerDlg::OnBnClickedDown)
	ON_BN_CLICKED(IDCANCEL, &CTextMergerDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_MERGE, &CTextMergerDlg::OnBnClickedMerge)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_FILES, &CTextMergerDlg::OnLvnColumnclickFiles)
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
	SetIcon(m_hIcon, FALSE);		// Set small icon

//	m_ctrlFiles.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_ctrlFiles.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 150);
	m_ctrlFiles.InsertColumn(1, _T("Path"), LVCFMT_LEFT, 230);

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
		dc.DrawIcon(x, y, m_hIcon);
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
	return static_cast<HCURSOR>(m_hIcon);
}

void CTextMergerDlg::OnBnClickedAdd()
{
	std::vector<CString> files = GetFiles();
	if (files.size() == 1)
	{
		LPTSTR sName = ::PathFindFileName(files[0]);
		CString sPath(files[0], sName - ((LPCTSTR)files[0]) - 1);
		InsertFile(m_ctrlFiles.GetItemCount(), sName, sPath);
	}
	else if (files.size() > 1)
	{
		for (int i = 1; i < (int)files.size(); ++i)
		{
			InsertFile(m_ctrlFiles.GetItemCount(), files[i], files[0]);
		}
	}
}
void CTextMergerDlg::OnDestroy()
{
	int cnt = m_ctrlFiles.GetItemCount();
	if (cnt > 0)
	{
		for (int i = cnt - 1; i >= 0; --i)
		{
			DeleteFile(i);
		}
	}
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
	for (int i = 0; i < m_ctrlFiles.GetItemCount(); ++i)
	{
	}
}

// Sort items by associated lParam
static int CALLBACK FilesCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CString* ps1 = (CString*)lParam1;
	CString* ps2 = (CString*)lParam2;

	return lParamSort ? lstrcmp(ps1->GetString(), ps2->GetString()) :
		lstrcmp(ps2->GetString(), ps1->GetString());
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
