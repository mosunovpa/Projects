/**
 * \file MainFrmModules.cpp
 *
 * \since 2006/10/11
 * \author pavel
 */

#include "StdAfx.h"
#include "MainFrmModules.h"
#include "ModuleLoader.h"

#define ITEM_CMD_OFFSET	32000

CMainFrmModules::CMainFrmModules(void)
{
	IModuleList* pModules = CModuleLoader::Get().GetModules();
	if (pModules->first())
	{
		do 
		{
			push_back(MODULE(&(pModules->current()), size() + 1, size()));
		} while (pModules->next());
	}
}

MODULE* CMainFrmModules::GetModule(DWORD mod_id)
{
	for (iterator it = begin(); it != end(); ++it)
		if (it->m_pModule->pModuleInfo->GetId() == mod_id)
			return &(*it);

	ATLASSERT(FALSE);
	return NULL;

}

// MODULE* CMainFrmModules::GetModuleByCmd(DWORD module_cmd)
// {
// 	for (iterator it = begin(); it != end(); ++it)
// 		if (it->m_nCmd == module_cmd - ITEM_CMD_OFFSET)
// 			return &(*it);
// 
// 	return NULL;
//}

DWORD CMainFrmModules::GetModuleCmd(DWORD mod_id)
{
	MODULE* pModule = GetModule(mod_id);
	ATLASSERT(pModule);
	return pModule->m_nCmd;
}

DWORD CMainFrmModules::GetModuleId(DWORD module_cmd)
{
	for (iterator it = begin(); it != end(); ++it)
		if (it->m_nCmd == module_cmd)
			return it->m_pModule->pModuleInfo->GetId();

	return 0;
}

BOOL CMainFrmModules::IsModuleCmd(DWORD module_cmd)
{
	return GetModuleId(module_cmd) != 0;
}

DWORD CMainFrmModules::GetModuleItemCmd(DWORD mod_id)
{
	MODULE* pModule = GetModule(mod_id);
	ATLASSERT(pModule);
	return pModule->m_nCmd + ITEM_CMD_OFFSET;
}

DWORD CMainFrmModules::GetModuleItemId(DWORD module_cmd)
{
	for (iterator it = begin(); it != end(); ++it)
		if (it->m_nCmd == module_cmd - ITEM_CMD_OFFSET)
			return it->m_pModule->pModuleInfo->GetId();

	return 0;
}

BOOL CMainFrmModules::IsModuleItemCmd(DWORD module_cmd)
{
	return GetModuleItemId(module_cmd) != 0;
}

void reset(MODULE& module)
{
	module.m_nCurrentFolder = 0;
}

void CMainFrmModules::Reset()
{
	std::for_each(begin(), end(), reset);
}