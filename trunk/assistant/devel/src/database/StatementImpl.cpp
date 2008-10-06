/**
 * \file StatementImpl.cpp
 *
 * \since 2006/09/20
 * \author pavel
 */

#include "StdAfx.h"
#include "StatementImpl.h"

//////////////////////////////////////////////////////////////////////////
//

CStatementImpl::CStatementImpl(sqlitepp::session& sess)
:	m_sess(sess), m_st(sess)
{
}

CStatementImpl::~CStatementImpl(void)
{
}

void CStatementImpl::sql(LPCTSTR sql)
{
	m_st.finalize();
	m_st.q().clear();
	m_st.q().sql(sql);
}

void CStatementImpl::into(int& value, LPCTSTR column_name)
{
	m_st.q().put(sqlitepp::into_binder_ptr(new sqlitepp::into_name_binder<int>(value, column_name)));
}

void CStatementImpl::into(long long& value, LPCTSTR column_name)
{
	m_st.q().put(sqlitepp::into_binder_ptr(new sqlitepp::into_name_binder<long long>(value, column_name)));
}

void CStatementImpl::into(CStringBuf& value, LPCTSTR column_name)
{
	m_st.q().put(sqlitepp::into_binder_ptr(new sqlitepp::into_name_binder<CStringBuf>(value, column_name)));
}

void CStatementImpl::use(int& value, LPCTSTR param_name)
{
	m_st.q().put(sqlitepp::use_binder_ptr(new sqlitepp::use_name_binder<int>(value, param_name)));
}

void CStatementImpl::use(long long& value, LPCTSTR param_name)
{
	m_st.q().put(sqlitepp::use_binder_ptr(new sqlitepp::use_name_binder<long long>(value, param_name)));
}

void CStatementImpl::use(CStringBuf& value, LPCTSTR param_name)
{
	m_st.q().put(sqlitepp::use_binder_ptr(new sqlitepp::use_name_any_binder<CStringBuf>(value, param_name)));
}

BOOL CStatementImpl::exec()
{
	return m_st.exec();
}

BOOL CStatementImpl::exec_script()
{
	BOOL bResult = TRUE;

	// parse script
	std::vector<_tstring> vStrList;
	_tstring sScript = m_st.q().sql();
	LPCTSTR pStart = sScript.c_str();
	for (LPCTSTR p = pStart; *p; ++p)
	{
		if (*p == ';')
		{
			_tstring s(pStart, p - pStart);
			if (!s.empty())
			{
				vStrList.push_back(s);
			}
			// find next start
			while (*p != 0 && (*p == ';' || *p == '\t' || *p == '\n' ||	*p == '\r' || *p == 0x20))
			{
				++p;
				pStart = p;
			}
		}
	}
	// run sql statements
	for (int i = 0; i != vStrList.size(); ++i)
	{
		m_st.q().sql(vStrList[i].c_str());
		m_st.exec();
	}
	return bResult;
}

void CStatementImpl::finalize()
{
	m_st.finalize();
}

int CStatementImpl::last_insert_rowid()
{
	return (int)m_sess.last_insert_rowid();

}