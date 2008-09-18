// Frames2Time.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Frames2Time.h"
#include "Frames2TimeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFrames2TimeApp

BEGIN_MESSAGE_MAP(CFrames2TimeApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CFrames2TimeApp construction

CFrames2TimeApp::CFrames2TimeApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CFrames2TimeApp object

CFrames2TimeApp theApp;


// CFrames2TimeApp initialization

BOOL CFrames2TimeApp::InitInstance()
{
	CWinApp::InitInstance();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CFrames2TimeDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
