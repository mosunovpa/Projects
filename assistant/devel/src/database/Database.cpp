/*!
 *	\file Database.cpp
 *
 *	\author Pavel Mosunov 
 *  \since 2005/10/14
 */

#include "stdafx.h"
#include "DatabaseImpl.h"
#include "resutils.h"

//////////////////////////////////////////////////////////////////////////
// functions

DATABASE_API Database::IDatabase* GetDatabase()
{
	return &CDatabaseImpl::Get();
}

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
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
