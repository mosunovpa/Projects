/**
 * \file errors.cpp
 *
 * \since 2006/03/17
 * \author pavel
 */


#include "stdafx.h"
#include "errors.h"

template <typename baseClass = IError>
class CExeptionT : public baseClass
{
public:
	CExeptionT(LPCTSTR szErrMsg) : m_sErrMsg(szErrMsg) {}
	virtual ~CExeptionT() {}

	virtual void Delete() { delete this; }
	virtual LPCTSTR What() { return this->m_sErrMsg.c_str(); }
private:
	_tstring m_sErrMsg;
};

typedef CExeptionT<IExeption> CAbort;
typedef CExeptionT<IError> CError;

void ThrowError(LPCTSTR msg)
{
	assert(FALSE);
	throw new CError(msg);
}
