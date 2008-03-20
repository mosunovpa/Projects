/*!
	\author Pavel Mosunov
	\since 2005-09-15

*/
#ifndef __WSTRING_H__
#define __WSTRING_H__

#include <string>
#include <assert.h>
#include <tchar.h>

// character type
#if defined(_UNICODE) || defined(UNICODE)
	typedef std::wstring _tstring;
	typedef std::wostringstream _tostringstream;
#else
	typedef std::string _tstring;
	typedef std::ostringstream _tostringstream;
#endif

#endif // __WSTRING_H__
