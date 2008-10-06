/**
 * \file strutils.h
 *
 * \since 2006/03/17
 * \author pavel
 */

#ifndef strutils_h__
#define strutils_h__

#include "wstring.h"

// character type
#if defined(_UNICODE) || defined(UNICODE) || defined(SQLITEPP_UNICODE)
#define ATOT(pStr, len) strutils::toWideString(pStr, len)
#define TTOA(pStr, len) strutils::toNarrowString(pStr, len)
#else
#define ATOT(pStr, len) _tstring(pStr, len)
#define TTOA(pStr, len) _tstring(pStr, len)
#endif

//////////////////////////////////////////////////////////////////////////
// Conversation functions
//
namespace strutils
{
 	_tstring guid2string(GUID const& szType);
 	_tstring format(LPCTSTR fmt, ...);
 	_tstring to_string(int val, size_t size = 20, int radix = 10);
	std::wstring toWideString(char const* pStr, int len);
	std::string toNarrowString(wchar_t const* pStr, int len);
	_tstring ExtractCaption(LPCTSTR text, UINT nLimit = 256);
} // namespace strutils

#endif // strutils_h__
