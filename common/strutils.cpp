/**
 * \file StrUtils.cpp
 *
 * \since 2006/03/17
 * \author pavel
 */

#include "stdafx.h"
#include "strutils.h"
#include "shlwapi.h"
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/find.hpp>

/*!
*/
_tstring strutils::format(LPCTSTR fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	_tstring resstr;
	resstr.resize(_vsctprintf(fmt, args) + 1);
	_vstprintf_s(&resstr[0], resstr.size(), fmt, args);

	va_end(args);

	return resstr;
}

/**
 * converts int to string
 */
_tstring strutils::to_string(int val, size_t size/* = 20*/, int radix/* = 10*/)
{
	_tstring res;
	res.resize(size);
	_itot_s(val, &res[0], size, radix);
	return _tstring(&res[0]); // need for adjusting of the result string size
}


std::wstring strutils::toWideString(char const* pStr, int len)
{
	assert(pStr != NULL) ; 
	assert(len > 0);
	if (pStr == NULL || len == 0)
	{
		return std::wstring();
	}

	// figure out how many wide characters we are going to get 
	int nChars = MultiByteToWideChar(CP_UTF8, 0, pStr, len, NULL, 0);

	assert(nChars != -1);
	if (nChars == 0 || nChars == 1)
		return L"" ;

	std::wstring buf;
	buf.resize(nChars);

	MultiByteToWideChar(CP_UTF8, 0, pStr, len, &buf[0], nChars);
	return std::wstring(&buf[0]); // invoke constructor for adjusting of buffer length
}
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */

std::string strutils::toNarrowString(wchar_t const* pStr, int len)
{
	assert(pStr != NULL) ; 
	assert(len > 0);
	if (pStr == NULL || len == 0)
	{
		return std::string();
	}

	// figure out how many narrow characters we are going to get 
	int bytes = (len/* + 1*/) * sizeof(wchar_t);

	// convert the wide string to a narrow string
	// nb: slightly naughty to write directly into the string like this
	std::string buf ;
	buf.resize(bytes) ;
	WideCharToMultiByte(CP_UTF8, 0, pStr, len, &buf[0], (int)buf.size(), NULL, NULL);
	return std::string(buf.c_str()); // invoke constructor for adjusting of buffer length
}

_tstring strutils::trim_string(LPCTSTR text, UINT nLimit /*= 256*/)
{
	_tstring res(text);
	boost::trim(res);
	_tstring::size_type pos = res.find(_tstring(_T("\r\n")));
	if (pos != _tstring::npos)
	{
		res.erase(pos);
	}
	pos = res.find(_tstring(_T("\n")));
	if (pos != _tstring::npos)
	{
		res.erase(pos);
	}
	if (res.size() > nLimit)
	{
		pos = res.rfind(_tstring(_T(" ")), nLimit);
		if (pos == _tstring::npos)
		{
			pos = nLimit;
		}
		res.erase(pos);
		boost::trim_right(res);
	}
	return res;
}