/**
 * \file dateutils.cpp
 *
 * \since 2006/12/08
 * \author pavel
 */
#include "stdafx.h"
#include "dateutils.h"

void dateutils::timebn::clear(_timeb& tb)
{
	memset(&tb, 0, sizeof(_timeb));
}
_timeb dateutils::timebn::getempty()
{
	_timeb tb;
	clear(tb);
	return tb;
}
int dateutils::timebn::compare(_timeb const & tbleft, _timeb const & tbright)
{
	if (tbleft.time != tbright.time)
	{
		return (tbleft.time < tbright.time) ? -1 : 1;
	}
	else if (tbleft.millitm != tbright.millitm)
	{
		return (tbleft.millitm < tbright.millitm) ? -1 : 1;
	}
	return 0;
}
bool dateutils::timebn::isempty(_timeb const & tb)
{
	return 0 == compare(tb, getempty());
}

_timeb dateutils::GetCurrentDateTime()
{
	_timeb tb;
	_ftime_s(&tb);
	return tb;
}

time_t dateutils::GetDate(time_t const& t)
{
	tm gt;
	gmtime_s(&gt, &t);
	gt.tm_sec = 0;
	gt.tm_min = 0;
	gt.tm_hour = 0;
	gt.tm_isdst = 0;

	return mktime(&gt);
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