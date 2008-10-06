// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__1D691405_A584_4498_A7D5_02268090A799__INCLUDED_)
#define AFX_STDAFX_H__1D691405_A584_4498_A7D5_02268090A799__INCLUDED_


// Insert your headers here
//#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

//#include <windows.h>

// TODO: reference additional headers your program requires here
// Change these values to use different versions
#define WINVER		0x0501
#define _WIN32_WINNT	0x0501
#ifndef _WIN32_IE
#define _WIN32_IE 0x0501
#endif
#define _RICHEDIT_VER	0x0200

#include <atlbase.h>

#if (_ATL_VER >= 0x0700)
#define _WTL_NO_CSTRING
#include <atlstr.h> 
#endif //(_ATL_VER >= 0x0700)

#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>

#include <atlframe.h>
#include <atlmisc.h> // for CString
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlctrlw.h>
#include <AtlCtrlx.h> // FOR "CWaitCursor"
#ifndef _INC_SHELLAPI
    #include <shellapi.h>
#endif 
#include <atlsplit.h>

// stl includes
#include <list>
#include <algorithm>

// app includes
#include "atlwinmisc.h"
#include "wexcept.h"
#include "errors.h"
#include "shared_ptr_2.h"

#if defined _M_IX86
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

//////////////////////////////////////////////////////////////////////////
// Test framework
// #ifdef DEBUG
// #pragma comment (lib, "cppunitd.lib")
// #else
// #pragma comment (lib, "cppunit.lib")
// #endif DEBUG

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__1D691405_A584_4498_A7D5_02268090A799__INCLUDED_)
