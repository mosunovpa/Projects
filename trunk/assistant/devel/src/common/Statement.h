
/**
 * \file Statement.h
 *
 * \since 2006/09/20
 * \author pavel
 */

#ifndef Statement_h__
#define Statement_h__

#include "releasable.h"
#include "shared_ptr_2.h"
#include "strbuf.h"

/**
* IStatement
*/
class IStatement : public IReleasable
{
public:
	virtual void sql(LPCTSTR sql) = 0;

	virtual void into(int& value, LPCTSTR column_name) = 0;
	virtual void into(long long& value, LPCTSTR column_name) = 0;
	virtual void into(CStringBuf& value, LPCTSTR column_name) = 0;

	virtual void use(int& value, LPCTSTR param_name) = 0;
	virtual void use(long long& value, LPCTSTR param_name) = 0;
	virtual void use(CStringBuf& value, LPCTSTR param_name) = 0;

	virtual BOOL exec() = 0;
	virtual BOOL exec_script() = 0;
	virtual void finalize() = 0;
	virtual int last_insert_rowid() = 0;
};

typedef shared_ptr_2<IStatement> IStatementPtr;

#endif // Statement_h__