/**
 * \file dateutils.h
 *
 * \since 2006/12/08
 * \author pavel
 */

#pragma once

#include "time.h"
#include "sys/timeb.h"

namespace dateutils
{
	namespace timebn
	{
		void clear(_timeb& tb);
		_timeb getempty();
		int compare(_timeb const & tbleft, _timeb const & tbright);
		bool isempty(_timeb const & tb);
	}

	_timeb GetCurrentDateTime();
	time_t GetDate(time_t const& t);
	_tstring ToString(time_t t, BOOL bLocal = TRUE);
	_tstring ToString(time_t t, LPCTSTR format, BOOL bLocal = TRUE);

} // namespace dateutils