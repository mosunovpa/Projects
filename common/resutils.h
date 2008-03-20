/**
 * \file resutils.h
 *
 * \since 2006/04/04
 * \author pavel
 */
#ifndef resutils_h__
#define resutils_h__

#include "wstring.h"
#include "strutils.h"

namespace resutils
{

// you should implement this function in your module
HINSTANCE GetResInstance();

_tstring resstring(UINT id);
_tstring resstring_fmt(UINT id, ...);
_tstring reshtml(UINT id);

} // namespace resutils

#define RESSTR(x)		resutils::resstring(x).c_str()
#define RESSTR_FMT		resutils::resstring_fmt

#endif // resutils_h__