/**
 * \file fileutils.cpp
 *
 * \since 2006/03/28
 * \author pavel
 */
#include "stdafx.h"
#include "fileutils.h"
#include "wstring.h"
#include "shlwapi.h"
#include "strutils.h"

BOOL fileutils::CreateDirectoryRecursive(LPCTSTR szPath, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	if (szPath == NULL)
	{
		return FALSE;
	}

	_tstring csDir = szPath;
	_tstring::size_type nStart, nIndex;
	
	// check if this is network path and pass computer name
	nStart = csDir.find(_T("\\\\"));
	if (nStart == 0)
	{
		nStart = csDir.find(_T("\\"), nStart + 2) + 1;
		if (nStart == _tstring::npos)
		{
			return FALSE;
		}
	}
	else
	{
		nStart = 0;
	}

	while ((nIndex = csDir.find(_T("\\"), nStart)) != -1)
	{
		_tstring csDirHead(csDir.c_str(), nIndex);
		if (!::PathIsDirectory(csDirHead.c_str()))
		{
			if (!::CreateDirectory(csDirHead.c_str(), NULL))
			{
				return FALSE;
			}
		}

		nStart = nIndex + 1;
	}

	if (!PathIsDirectory(szPath))
	{
		if (!::CreateDirectory(szPath, NULL))
		{
			return FALSE;
		}
	}

	return TRUE;
}

ULONGLONG fileutils::GetFileSize(LPCTSTR szFilepath)
{
	if(szFilepath != NULL)
	{
		HANDLE hFile  = ::CreateFile(szFilepath, 0, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hFile != INVALID_HANDLE_VALUE)
		{
			LARGE_INTEGER liSize;
			if(::GetFileSizeEx(hFile, &liSize))
			{
				return liSize.QuadPart;
			}
		}
	}
	return 0;
}


/**
* returns file name with extention
*/
_tstring fileutils::file_name(LPCTSTR szFilePath)
{
	return ::PathFindFileName(szFilePath);
}
/**
*/
BOOL fileutils::Write(LPCTSTR szFilepath, _tstring s)
{
	BOOL res = 0;
	if(szFilepath != NULL)
	{
		HANDLE hFile  = ::CreateFile(szFilepath, GENERIC_READ | GENERIC_WRITE, 
			0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hFile != INVALID_HANDLE_VALUE)
		{
			::SetFilePointer(hFile,0,0,FILE_END);
			DWORD written;
			std::string ns = strutils::toNarrowString(s.c_str(), (int)s.size());
			res = ::WriteFile(hFile, ns.c_str(), (DWORD)ns.size(), &written, NULL);
			::CloseHandle(hFile);
		}
	}
	return res;
}

/**
*/
BOOL fileutils::WriteLn(LPCTSTR szFilepath, _tstring s)
{
	return fileutils::Write(szFilepath, s + _T("\r\n"));
}
