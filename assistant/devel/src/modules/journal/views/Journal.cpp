// Journal.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "resource.h"

#include "Journal.h"
#include "JournalModuleImpl.h"

CAppModule _Module;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			{
			HRESULT hRes = _Module.Init(NULL, (HINSTANCE) hModule);
			ATLASSERT(SUCCEEDED(hRes));	// Don't call important functions inside ASSERT.
			}
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			_Module.Term();
			break;
    }
    return TRUE;
}

JOURNAL_API Module::IModuleInfo* GetModuleInfo(Assist::IApplication* pApplication)
{
	g_Module.m_pApplication = pApplication;
	return &g_Module;
}
