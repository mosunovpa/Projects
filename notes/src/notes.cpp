
#include "stdafx.h"
#include "resource.h"
#include "resutils.h"
#include "Application.h"
#include "apputils.h"
#include "atlscintilla.h"
#include "noteshook.h"
#include "fileutils.h"
#include "user_messages.h"

CAppModule _Module;

namespace resutils
{
	HINSTANCE GetResInstance()
	{
		return _Module.GetResourceInstance();
	}
}

namespace errors
{
	int ErrorMessage(LPCTSTR msg, HWND hWnd /*= NULL*/)
	{
		return AtlMessageBox(hWnd, msg, RESSTR(IDR_MAINFRAME), MB_OK | MB_ICONERROR);
	}
}

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // report memory leaks

	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	try
	{
		CApplication::Get().CreateAppWindow();
		int nRet = theLoop.Run();
		_Module.RemoveMessageLoop();
		return nRet;
	}
	CATCH_ALL_ERRORS(NULL) 
	return 0;
}

HHOOK g_mouse_hook = 0;

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode < 0)  // не обрабатываем сообщение
        return CallNextHookEx(g_mouse_hook, nCode, wParam, lParam);
	
	if (wParam == WM_LBUTTONDBLCLK && nCode == HC_ACTION)
	{
		const int len = 100;
		TCHAR class_name[len];
		LPMOUSEHOOKSTRUCT pmhs = (LPMOUSEHOOKSTRUCT)lParam;
		if (GetClassName(pmhs->hwnd, class_name, len))
		{
			if (lstrcmp(class_name, NOTE_WND_CLASS_NAME) == 0)
			{
				SendMessage(pmhs->hwnd, WMU_LBUTTONDBLCLK, wParam, lParam);
			}
		}
	}
    return CallNextHookEx(g_mouse_hook, nCode, wParam, lParam);
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	if (apputils::SetOneInstance(_T("706CB7A9-A3B4-40e2-AB12-9E5B0A3D4C8B")))
	{
		// application already started
		return 0;
	}

	HRESULT hRes = ::CoInitialize(NULL);
	// If you are running on NT 4.0 or higher you can use the following call instead to 
	// make the EXE free threaded. This means that calls come in on a random RPC thread.
	//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

#ifdef SCINTILLA
	CScintillaAutoRegister ScintillaReg;
#else
	HINSTANCE hInstRich = ::LoadLibrary(CRichEditCtrl::GetLibraryName());
	ATLASSERT(hInstRich != NULL);
#endif

	SetHook();

	/* установить хук на мышку, чтоб поймать двойной клик на системной иконке */
	g_mouse_hook = SetWindowsHookEx(WH_MOUSE, MouseProc, NULL, GetCurrentThreadId());

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();

	RemoveHook();

	UnhookWindowsHookEx(g_mouse_hook);

	::CoUninitialize();

	return nRet;
}
