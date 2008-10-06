/**
 * \file stdstr.cpp
 *
 * \since 2006/04/04
 * \author pavel
 */

#include "stdafx.h"
#include "resutils.h"

static HMODULE s_res_handle = NULL;

void resutils::set_handle(HMODULE h)
{
	s_res_handle = h;
}

HMODULE resutils::get_handle()
{
	return s_res_handle; 
}

_tstring resutils::resstring(UINT id)
{
	_tstring resstr;
	int nSize = 0;
	int nLen = 0;
	do 
	{
		nSize += 256;
		resstr.resize(nSize);
		nLen = ::LoadString((HINSTANCE)s_res_handle, id, &resstr[0], nSize);

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
	HRSRC hrsrc = ::FindResource(s_res_handle, MAKEINTRESOURCE(id), RT_HTML);
	if (hrsrc)
	{
		HGLOBAL hdata = ::LoadResource(s_res_handle, hrsrc);
		if (hdata)
		{
			LPBYTE lpBuff = (LPBYTE)::LockResource(hdata);
			if (lpBuff)
			{
				UINT cb = ::SizeofResource(s_res_handle, hrsrc);
				resstr = ATOT((LPCSTR)lpBuff, cb);
				UnlockResource(hdata);
			}
			::FreeResource(hdata);
		}
	}
	return resstr;
}
