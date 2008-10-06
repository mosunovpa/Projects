/*!
 * \file DatabaseImpl.h
 * 
 * \author Pavel Mosunov
 * \since 2005-8-2
 */

#ifndef __DATABASEIMPL_H__
#define __DATABASEIMPL_H__

#include "Database.h"
#include "DataFile.h"
#include "SafePtr.h"
#include "simplesinglton.h"

class CDatabaseImpl : public CSimpleSinglton<CDatabaseImpl>, public Database::IDatabase
{
	friend class CSimpleSinglton<CDatabaseImpl>;

protected:
	CDatabaseImpl(void);
	virtual ~CDatabaseImpl(void);

public:
	// IDatabase interface
	virtual Database::IDataFile* OpenDataFile(const TCHAR* pFileName);
	virtual void CloseDataFile(Database::IDataFile* pFile);

	// implementation

private:
	std::vector<CDataFile*> m_files;
};

#endif //__DATABASEIMPL_H__
