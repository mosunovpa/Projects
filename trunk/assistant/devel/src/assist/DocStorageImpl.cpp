/**
* \file DocStorageImpl.cpp
*
* \since 2006/09/06
* \author pavel
*/

#include "StdAfx.h"
#include "DocStorageImpl.h"
#include "statement.h"
#include "guiutils.h"
#include "Database.h"
#include "ModuleInfo.h"
#include "Defines.h"
#include "dateutils.h"
#include "strutils.h"

//////////////////////////////////////////////////////////////////////////
// CDocStorageImpl

CDocStorageImpl::CDocStorageImpl(Module::IModuleInfo* pModuleInfo, Database::IDataFile* pDataFile)
:	m_pDataFile(pDataFile),
	m_pModuleInfo(pModuleInfo)
{
}

int CDocStorageImpl::GetStorageVer() const
{
	try
	{
		IStatementPtr st = m_pDataFile->GetStatement();
		int ver = 0;
		st->sql(_T("select version from adx_modules where id = :id"));
		st->into(ver, _T("version"));
		int id = m_pModuleInfo->GetId();
		st->use(id, _T(":id")); 
		st->exec();
		return ver;
	}
	RAISE()

	return 0;
}

void CDocStorageImpl::Update(LPCTSTR szScript, int nVer)
{
	try
	{
		IStatementPtr st = m_pDataFile->GetStatement();

		if (szScript != NULL)
		{
			st->sql(szScript);
			st->exec_script();
		}
		
		// set new version number
		int ver = 0;
		int id = m_pModuleInfo->GetId();
		CStringBuf szName(m_pModuleInfo->GetName());
		st->sql(_T("select version from adx_modules where id = :id"));
		st->into(ver, _T("version"));
		st->use(id, _T(":id"));
		st->exec();

		if (ver == 0)
		{
			// first time
 			// append folder
 
 			time_t t = dateutils::GetCurrentDate();
 			st->sql(_T("insert into adx_items (created, name, module, folder) values (:created, :name, :module, 1)"));
			st->use(t, _T(":created"));
 			st->use(szName, _T(":name"));
 			st->use(id, _T(":module"));
 			st->exec();

			// insert to modules
			st->sql(_T("insert into adx_modules (id, name, version) values (:id, :name, :ver)"));
			st->use(szName, _T(":name"));
			st->use(id, _T(":id"));
			st->use(nVer, _T(":ver"));
			st->exec();
		}
		else
		{
			st->sql(_T("update adx_modules set version = :ver where id = :id"));
			st->use(id, _T(":id"));
			st->use(nVer, _T(":ver"));
			st->exec();
		}
	}
	RAISE()
}
