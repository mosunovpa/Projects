// noteshook.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "noteshook.h"


#ifdef _MANAGED
#pragma managed(push, off)
#endif

HWND g_hwnd = 0;
DWORD g_cmd = 0;
HMODULE g_hModule = 0;
HHOOK g_hook = 0;

//#pragma data_seg(".SHARED")
//int entries[10] = { 0 };
//#pragma data_seg()

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
	if (nCode == HSHELL_WINDOWACTIVATED) 
	{
        // snip 
    }
    return CallNextHookEx(0, nCode, wParam, lParam); 
}

HWND GetPrevActiveWindow()
{
	return 0;
}

NOTESHOOK_API void SetHook(HWND hwnd, DWORD cmd)
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

