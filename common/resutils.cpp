/**
 * \file stdstr.cpp
 *
 * \since 2006/04/04
 * \author pavel
 */

#include "stdafx.h"
#include "resutils.h"

_tstring resutils::resstring(UINT id)
{
	_tstring resstr;
	int nSize = 0;
	int nLen = 0;
	do 
	{
		nSize += 256;
		resstr.resize(nSize);
		nLen = ::LoadString(GetResInstance(), id, &resstr[0], nSize);

	} while (nLen == nSize - 1);
	
	return resstr;
}

_tstring resutils::resstring_fmt(UINT id, ...)
{
	_tstring fstr = resstring(id);

	va_list args;
	va_start(args, id);

	_tstring resstr;
	resstr.resize(_vsctprintf(fstr.c_str(), args) + 1);
	_vstprintf_s(&resstr[0], resstr.size(), fstr.c_str(), args);

	va_end(args);

	return resstr;
}

_tstring resutils::reshtml(UINT id)
{
	_tstring resstr;
	HRSRC hrsrc = ::FindResource(GetResInstance(), MAKEINTRESOURCE(id), RT_HTML);
	if (hrsrc)
	{
		HGLOBAL hdata = ::LoadResource(GetResInstance(), hrsrc);
		if (hdata)
		{
			LPBYTE lpBuff = (LPBYTE)::LockResource(hdata);
			if (lpBuff)
			{
				UINT cb = ::SizeofResource(GetResInstance(), hrsrc);
				resstr = ATOT((LPCSTR)lpBuff, cb);
				UnlockResource(hdata);
			}
			::FreeResource(hdata);
		}
	}
	return resstr;
}
