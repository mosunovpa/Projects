// noteshook.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "noteshook.h"
#include "strutils.h"
#include "fileutils.h"

#ifdef _MANAGED
#pragma managed(push, off)
#endif

#pragma data_seg(".SHARED")

HWND g_curwnd = 0;
HMODULE g_hModule = 0;
HHOOK g_hook = 0;

#pragma data_seg()
#pragma comment(linker, "/section:.SHARED,rws")

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		g_hModule = hModule;
	}
    return TRUE;
}

LRESULT CALLBACK ShellProc(
  __in  int nCode,
  __in  WPARAM wParam,
  __in  LPARAM lParam
)
{
	if ((nCode == HSHELL_WINDOWACTIVATED)) 
	{
		if (wParam)
		{
			g_curwnd = (HWND)wParam;
		}
//		fileutils::WriteLn(_T("C:\\log.log"), strutils::to_string((int)wParam));
    }
    return CallNextHookEx(0, nCode, wParam, lParam); 
}

NOTESHOOK_API HWND GetPrevActiveWindow()
{
	return g_curwnd;
}

NOTESHOOK_API void SetHook()
{
	g_hook = SetWindowsHookEx(WH_SHELL, ShellProc, g_hModule, 0);
}

NOTESHOOK_API void RemoveHook()
{
	if (g_hook)
	{
		UnhookWindowsHookEx(g_hook);
	}
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

