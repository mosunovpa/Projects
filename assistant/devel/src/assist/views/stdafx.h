// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#ifndef __STDAFX_H__
#define __STDAFX_H__

//#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

// Change these values to use different versions
#define WINVER		0x0501
#define _WIN32_WINNT	0x0501
#define _WIN32_IE	0x0501
#define _RICHEDIT_VER	0x0200
#define OEMRESOURCE

#include <atlbase.h>


#if (_ATL_VER >= 0x0700)
#define _WTL_NO_CSTRING
#include <atlstr.h> 
#endif //(_ATL_VER >= 0x0700)


#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>

#include <atlframe.h>
#include <atlmisc.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlctrlw.h>
#include <atlctrlx.h>
#include <atlcoll.h>
#include <atlscrl.h>
#include <winuser.h>
#include <atlpath.h>
#include <atlsplit.h>
#include <atlcrack.h>

/**
 * std headers
 */
#include <list>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <functional>
#include <string>

#include "atlwinmisc.h"
#include "wstring.h"
#include "wexcept.h"
#include "shared_ptr_2.h"
#include "errors.h"
#include "strutils.h"
#include "resutils.h"

#if defined _M_IX86
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

// #ifdef DEBUG
// #pragma comment (lib, "cppunitd.lib")
// #else
// #pragma comment (lib, "cppunit.lib")
// #endif DEBUG

#endif // __STDAFX_H__