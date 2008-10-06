/*!
\file Database.h
Database interface

\author Pavel Mosunov
\since 2005-09-19

*/

#ifndef __DATABASE_H__
#define __DATABASE_H__

#include "rowiterator.h"
#include "statement.h"

#define DB_VER 0x0100

//! Database classes
namespace Database
{
	/**
	 * 
	 */
	class IDataFile
	{
	public:
		virtual const TCHAR* GetFileName() const = 0;
		virtual IStatement* GetStatement() = 0;
		virtual bool IsEmpty() const = 0;
	protected:
		virtual ~IDataFile() {}
	};

	/**
	* IDatabase
	*/
	class IDatabase
	{
	public:
		virtual IDataFile* OpenDataFile(const TCHAR* pFileName) = 0;
		virtual void CloseDataFile(IDataFile* pFile) = 0;

	protected:
		virtual ~IDatabase() {}
	};
} // namespace Database

//////////////////////////////////////////////////////////////////////////
// export functions

#ifdef DATABASE_EXPORTS
#define DATABASE_API __declspec(dllexport)
#else
#define DATABASE_API __declspec(dllimport)
#endif

extern "C" 
{
/**
* return pointer to IDatabase instance;
*/
DATABASE_API Database::IDatabase* GetDatabase();

} // extern "C" 

#endif //__DATABASE_H__
