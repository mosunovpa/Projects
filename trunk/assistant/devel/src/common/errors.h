/**
 * \file errors.h
 *
 * \since 2006/03/17
 * \author pavel
 */

#ifndef errors_h__
#define errors_h__

#include "wexcept.h"
#include "wstring.h"
#include "strutils.h"

void ThrowError(LPCTSTR msg);

class IExeption
{
public:
	virtual void Delete() = 0;
	virtual LPCTSTR What() = 0;
protected:
	virtual ~IExeption() {}
};

class IError : public IExeption
{
};

#define RAISE() \
	catch (IExeption* ) { throw; } \
	catch (wexception e) { ThrowError(e.what()); } \
	catch (std::runtime_error e) { ThrowError(ATOT(e.what(), strlen(e.what())).c_str()); } \
	catch (std::exception e) { ThrowError(ATOT(e.what(), strlen(e.what())).c_str()); } \
	catch (...) { ThrowError(_T("Unknown error")); } \

#endif // errors_h__
