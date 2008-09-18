// Frames2TimeDlg.cpp : implementation file
//

#include "stdafx.h"
#include <utility>
#include "Frames2Time.h"
#include "Frames2TimeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// helpers

typedef std::pair<CTime, int> CTimeFrames;

/* concert time + rest frames to the frames*/
static int ConvertTime(CTime const& time, int nTimeFrames)
{
	return (time.GetHour() * 3600 + time.GetMinute() * 60 + time.GetSecond()) * 25 + nTimeFrames;
}

/* convert frames to the time and rest frames*/
static CTimeFrames ConvertFrames(int nFrames)
{
	int nSeconds = nFrames / 25;
	int nRestFrames = nFrames % 25;
	int nMinutes = nSeconds / 60;
	int nRestSeconds = nSeconds % 60;
	int nHours = nMinutes / 60;
	int nRestMinutes = nMinutes % 60;
	int nRestHours = nHours % 24;
	return CTimeFrames(CTime(2000,1,1,nRestHours,nRestMinutes,nRestSeconds), nRestFrames);
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


// CFrames2TimeDlg dialog

CFrames2TimeDlg::CFrames2TimeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFrames2TimeDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFrames2TimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_FRAMES, m_ctrlFrames);
	DDX_Control(pDX, IDC_TIME, m_ctrlTime);
	DDX_Control(pDX, IDC_SPIN_TIME_FRAMES, m_ctrlTimeFramesPos);
	DDX_Control(pDX, IDC_EDIT_TIME_FRAME, m_ctrlTimeFrames);
}

BEGIN_MESSAGE_MAP(CFrames2TimeDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCLOSE, &CFrames2TimeDlg::OnBnClickedClose)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_TIME, &CFrames2TimeDlg::OnDtnDatetimechangeTime)
	ON_EN_CHANGE(IDC_EDIT_FRAMES, &CFrames2TimeDlg::OnEnChangeEditFrames)
	ON_EN_CHANGE(IDC_EDIT_TIME_FRAME, &CFrames2TimeDlg::OnEnChangeEditTimeFrames)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CFrames2TimeDlg message handlers

BOOL CFrames2TimeDlg::OnInitDialog()
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
	
	m_ctrlFrames.SetRange(0, 2159999);
	m_ctrlFrames.SetWindowText(_T("0"));
	CTime tm(2000,1,1,0,0,0);
	m_ctrlTime.SetTime(&tm);
	m_ctrlTimeFrames.SetRange(0, 25);
	m_ctrlTimeFramesPos.SetRange(0, 25);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFrames2TimeDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFrames2TimeDlg::OnPaint()
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
HCURSOR CFrames2TimeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFrames2TimeDlg::OnBnClickedClose()
{
	PostMessage(WM_CLOSE);
}

/* time of time frames changed */
void CFrames2TimeDlg::OnDtnDatetimechangeTime(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	
	SetFrames();

	*pResult = 0;
}

/* time or time frames changed */
void CFrames2TimeDlg::OnEnChangeEditTimeFrames()
{
	SetFrames();
}

/* frames changed */
void CFrames2TimeDlg::OnEnChangeEditFrames()
{
	CString csFrames;
	m_ctrlFrames.GetWindowText(csFrames);

	CTimeFrames timeFrames = ConvertFrames(_tstoi(csFrames));
	CTime timeCur;
	m_ctrlTime.GetTime(timeCur);
	if (timeCur != timeFrames.first)
	{
		m_ctrlTime.SetTime(&timeFrames.first);
	}
	if (m_ctrlTimeFramesPos.GetPos() != timeFrames.second)
	{
		m_ctrlTimeFramesPos.SetPos(timeFrames.second);
	}
}

void CFrames2TimeDlg::SetFrames()
{
	if (::IsWindow(m_ctrlTime.m_hWnd))
	{
		CTime tm;
		m_ctrlTime.GetTime(tm);
		BOOL bErr = FALSE;
		int nPos = m_ctrlTimeFramesPos.GetPos32(&bErr);
		if (!bErr)
		{
			int nFrames = ConvertTime(tm, nPos);
			CString csFrames;
			csFrames.Format(_T("%d"), nFrames);

			CString csCurFrames;
			m_ctrlFrames.GetWindowText(csCurFrames);
			if (csCurFrames != csFrames)
			{
				m_ctrlFrames.SetWindowText(csFrames);
			}
		}
	}
}

void CFrames2TimeDlg::OnClose()
{
	EndDialog(IDOK);
}

