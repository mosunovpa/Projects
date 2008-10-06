/*!
 * \file DatabaseImpl.cpp
 * 
 * \author Pavel Mosunov
 * \since 2005-8-2
 */

#include "StdAfx.h"
#include "resource.h"
#include "resutils.h"
#include "DatabaseImpl.h"

CDatabaseImpl::CDatabaseImpl(void)
{
}

CDatabaseImpl::~CDatabaseImpl(void)
{
	for (std::vector<CDataFile*>::iterator it = m_files.begin(); it != m_files.end(); ++it)
	{
		delete *it;
	}
}

/**
* Create new data file.
*/
Database::IDataFile* CDatabaseImpl::OpenDataFile(const TCHAR* pFileName)
{
	std::auto_ptr<CDataFile> spFile(new CDataFile());
	try
	{
		spFile->Open(pFileName);
	}
	RAISE()

	CDataFile* pFile = spFile.release();
	m_files.push_back(pFile);

	return pFile;
}

void CDatabaseImpl::CloseDataFile(Database::IDataFile* pFile)
{
	for (std::vector<CDataFile*>::iterator it = m_files.begin(); it != m_files.end(); ++it)
	{
		if (*it == pFile)
		{
			delete *it;
			m_files.erase(it);
			return;
		}
	}
}
