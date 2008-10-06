/**
 * \file StatementImpl.h
 *
 * \since 2006/09/20
 * \author pavel
 */

#pragma once

#include <map>
#include <vector>
#include <utility>
#include "Database.h"
#include "sqlitepp_ex.h"
#include "Statement.h"

class CStatementImpl : public IStatement
{
public:
	CStatementImpl(sqlitepp::session& sess);
	virtual ~CStatementImpl(void);

	virtual void sql(LPCTSTR sql) override;
	virtual void into(int& value, LPCTSTR column_name) override;
	virtual void into(long long& value, LPCTSTR column_name) override;
	virtual void into(CStringBuf& value, LPCTSTR column_name) override;

	virtual void use(int& value, LPCTSTR param_name) override;
	virtual void use(long long& value, LPCTSTR column_name) override;
	virtual void use(CStringBuf& value, LPCTSTR param_name) override;

	virtual BOOL exec() override;
	virtual BOOL exec_script() override;
	virtual void finalize() override;
	virtual void release() { delete this; }

	virtual int last_insert_rowid() override;

private:
	sqlitepp::session& m_sess;
	sqlitepp::statement m_st;
};
