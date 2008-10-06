/*!
 * \file CModuleLoader::Get().cpp
 * 
 * \author Pavel Mosunov
 * \since 2005-5-10
 */

#include "StdAfx.h"
#include "ModuleLoader.h"
#include "resource.h"
#include "resutils.h"
#include "TrashModuleImpl.h"
#include "ApplicationImpl.h"

//////////////////////////////////////////////////////////////////////////
// CModule

/**
 * constructor
 */
CModule::CModule() 
:	hModule(NULL), 
	pModuleInfo(NULL)
{
}

/**
 *destructor
 */
CModule::~CModule()
{
}

//////////////////////////////////////////////////////////////////////////
// CModuleLoader

/**
 * constructor. Loads modules.
 */
CModuleLoader::CModuleLoader()
{
}

CModuleLoader::CModuleLoader( CModuleLoader const& right )
{
}

/**
 * destructor
 */
CModuleLoader::~CModuleLoader()
{
	// clear module main views list
	for (CModuleList::iterator it = m_Modules.m_list.begin();
		it != m_Modules.m_list.end(); ++it)
	{
		if (it->hModule != NULL && it->hModule != _Module.GetResourceInstance())
			::FreeLibrary(it->hModule);
	}
}

/**
 * 
 */
void CModuleLoader::LoadModules()
{
	TCHAR szModuleName[MAX_PATH + 1];
	DWORD nCount = ::GetModuleFileName(NULL, szModuleName, MAX_PATH);
	ATLASSERT(nCount != 0);
	if (nCount > 0)
	{
		CPath ModulePath(szModuleName);
		ModulePath.RemoveFileSpec();
		ModulePath.Append(_T("modules\\"));

		WIN32_FIND_DATA FindFileData;
		HANDLE hFile = ::FindFirstFile(ModulePath + _T("*.dll"), &FindFileData);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			// enumerate modules
			do 
			{
				CString csModuleName = FindFileData.cFileName;
				CModule module;
				if (LoadModule(ModulePath + (LPCTSTR)csModuleName, module))
				{
					m_Modules.m_list.push_back(module);
				}
			}
			while (::FindNextFile(hFile, &FindFileData));
			::FindClose(hFile);
		}
	}

	if (m_Modules.m_list.size() == 0)
	{
		ThrowError(RESSTR(IDS_NO_MODULES_FOUND));
	}	
	
	// add trash module
	CModule module;
	module.pModuleInfo = &CTrashModuleImpl::Get();
	module.hModule = _Module.GetResourceInstance();
	m_Modules.m_list.push_back(module);
}

/**
 *
 */
IModuleList* CModuleLoader::GetModules()
{
	return &m_Modules;
}

/**
 * Load module and return TRUE if module has been loaded and module is valid.
 */
BOOL CModuleLoader::LoadModule(const CString& csModuleName, CModule& module)
{
	module.hModule = ::LoadLibrary(csModuleName);
	if (module.hModule != NULL)
	{
		try
		{
			Module::LPGETMODULEINFOPROC pGetModuleInfo = 
				(Module::LPGETMODULEINFOPROC)::GetProcAddress(module.hModule, "GetModuleInfo");
			if (pGetModuleInfo != NULL)
			{
				module.pModuleInfo = (*pGetModuleInfo)(&CApplicationImpl::Get());
				if (module.pModuleInfo != NULL)
					return TRUE;
			}
			// invalid module or NULL module info in module
		}
		CATCH_ALL_ERRORS(NULL) 

		::FreeLibrary(module.hModule);
	}
	return FALSE;
}

/**
 * return pointer to module info by module type
 */
CModule* CModuleLoader::FindModule(DWORD mod_id)
{
	for (CModuleList::iterator it = m_Modules.m_list.begin(); it != m_Modules.m_list.end(); ++it)
		if (it->pModuleInfo->GetId() == mod_id)
			return &(*it);

	return NULL;
}


