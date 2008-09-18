// Frames2Time.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CFrames2TimeApp:
// See Frames2Time.cpp for the implementation of this class
//

class CFrames2TimeApp : public CWinApp
{
public:
	CFrames2TimeApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CFrames2TimeApp theApp;