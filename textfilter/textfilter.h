// textfilter.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Ctextfilter.App:
// See textfilter.cpp for the implementation of this class
//

class CTextFilterApp : public CWinApp
{
public:
	CTextFilterApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CTextFilterApp theApp;