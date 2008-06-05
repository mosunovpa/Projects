/**
 * \file dateutils.cpp
 *
 * \since 2006/12/08
 * \author pavel
 */
#include "stdafx.h"
#include "dateutils.h"

time_t dateutils::GetCurrentDate()
{
	time_t tt;
	time(&tt);
	return tt;
}

_tstring dateutils::ToString(time_t t, BOOL bLocal /*= TRUE*/)
{
	return ToString(t, _T("%Y-%m-%d %H:%M:%S"), bLocal);
}

_tstring dateutils::ToString(time_t t, LPCTSTR format, BOOL bLocal /*= TRUE*/)
{
	tm gt;
	if (bLocal)
		localtime_s(&gt, &t);
	else
		gmtime_s(&gt, &t);

	TCHAR sDest[20];
	_tcsftime(sDest, sizeof(sDest)/sizeof(TCHAR), format, &gt);

	return _tstring(sDest);
}