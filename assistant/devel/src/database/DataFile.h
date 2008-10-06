/*!
 * \file DataFile.h
 * 
 * \author Pavel Mosunov
 * \since 2005-8-2
 */

#ifndef __DATAFILE_H__
#define __DATAFILE_H__

#include "Database.h"
#include "session.hpp"

class CDataFile : public Database::IDataFile
{
public:
	CDataFile();
	virtual ~CDataFile(void);

	// IDataFile interface

	// implementation

	void Open(const TCHAR* pFileName);
	void Close();

	virtual const TCHAR* GetFileName() const;
	virtual IStatement* GetStatement();
	virtual bool IsEmpty() const;

private:
	_tstring m_sFileName;
	sqlitepp::session m_sess;

};

#endif //__DATAFILE_H__
