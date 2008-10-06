// Notes.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "resource.h"
#include "assistres.h"
#include "resutils.h"
#include "Notes.h"
#include "NotesModuleImpl.h"

CAppModule _Module;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	// init resource module
	resutils::set_handle((HMODULE)hModule);
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
NOTES_API Module::IModuleInfo* GetModuleInfo(Assist::IApplication* pApplication)
{
	return &CNotesModuleImpl::Get(pApplication); //GetModule(pApplication);
}

