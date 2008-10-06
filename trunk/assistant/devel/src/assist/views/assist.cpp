// assist.cpp : main source file for assist.exe
//

#include "stdafx.h"

#include "ModuleLoader.h"
#include "ApplicationImpl.h"
#include "guiutils.h"
#include "StrUtils.h"

CAppModule _Module;

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // report memory leaks

	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	try
	{
		CModuleLoader::Get().LoadModules();
		try
		{
			CApplicationImpl::Get().OpenLastDataFile();
		}
		catch(...)
		{
			return 0;
		}

		// Create a hidden window
		if (!CApplicationImpl::Get().CreateAppWindow())
			return 0;

		int nRet = theLoop.Run();

		_Module.RemoveMessageLoop();
		return nRet;
	}
	CATCH_ALL_ERRORS(NULL) 

	return 0;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
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

	HINSTANCE hInstRich = ::LoadLibrary(CRichEditCtrl::GetLibraryName());
	ATLASSERT(hInstRich != NULL);

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
