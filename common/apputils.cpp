#include "stdafx.h"
#include "apputils.h"

BOOL apputils::SetOneInstance(LPCTSTR szName)
{
	HANDLE hMutex=NULL;
	BOOL bFound=FALSE;
	hMutex = ::CreateMutex(NULL, TRUE, szName);
	if(GetLastError() == ERROR_ALREADY_EXISTS)
		bFound = TRUE;
	if(hMutex)
		::ReleaseMutex(hMutex);
	return bFound;
}
