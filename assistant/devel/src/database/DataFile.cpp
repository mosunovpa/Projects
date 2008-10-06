/*!
 * \file DataFile.cpp
 * 
 * \author Pavel Mosunov
 * \since 2005-8-2
 */

#include "StdAfx.h"
#include "resource.h"
#include "datafile.h"
#include "fileutils.h"
#include "resutils.h"
#include "statement.hpp"
#include "shlwapi.h"
#include "FoldersImpl.h"
#include "ItemsImpl.h"
#include "ModulesImpl.h"
#include "StatementImpl.h"

//////////////////////////////////////////////////////////////////////////
// CDataFile

CDataFile::CDataFile()
{
}

CDataFile::~CDataFile(void)
{
	Close();
}

bool CDataFile::IsEmpty() const
{
	if (m_sFileName.empty())
		ThrowError(_T("file is not opened"));

	return fileutils::GetFileSize(GetFileName()) == 0;
}

/**
 * 
 */
void CDataFile::Open(const TCHAR* pFileName)
{
	try
	{
		Close();
		BOOL bNewFile = !::PathFileExists(pFileName);
		try
		{
			m_sess.open(pFileName);
			m_sFileName = pFileName;
		}
		catch (...)
		{
			Close();
			if (bNewFile)
			{
				::DeleteFile(pFileName);
			}
			throw;
		}
	}
	RAISE()
}

/**
 * 
 */
void CDataFile::Close()
{
	m_sess.close();
	m_sFileName.clear();
}

/**
 * 
 */
const TCHAR* CDataFile::GetFileName() const
{
	return m_sFileName.c_str();
}

/**
 * 
 */
IStatement* CDataFile::GetStatement()
{
	return new CStatementImpl(m_sess);
}
