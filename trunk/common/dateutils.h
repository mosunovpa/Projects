/**
 * \file dateutils.h
 *
 * \since 2006/12/08
 * \author pavel
 */
#include "time.h"

namespace dateutils
{
	time_t GetCurrentDate();
	_tstring ToString(time_t t, BOOL bLocal = TRUE);
	_tstring ToString(time_t t, LPCTSTR format, BOOL bLocal = TRUE);

} // namespace dateutils