/**
 * \file DocStorageImpl.h
 *
 * \since 2006/09/06
 * \author pavel
 */

#pragma once

#include "Application.h"

namespace Database
{
class IDataFile;
}

namespace Module
{
class IModuleInfo;
}

//////////////////////////////////////////////////////////////////////////
// CDocStorageImpl

class CDocStorageImpl : public Assist::IDocStorage
{
public:
	CDocStorageImpl(Module::IModuleInfo* pModuleInfo, Database::IDataFile* pDataFile);
	virtual int GetStorageVer() const override;
	virtual void Update(LPCTSTR szScript, int nVer) override;
private:
	Database::IDataFile* m_pDataFile;
	Module::IModuleInfo* m_pModuleInfo;
};
