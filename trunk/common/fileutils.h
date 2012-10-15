/**
 * \file fileutils.h
 *
 * \since 2006/03/28
 * \author pavel
 */
#ifndef fileutils_h__
#define fileutils_h__

#include "wstring.h"
#include <list>

namespace fileutils
{
BOOL CreateDirectoryRecursive(LPCTSTR szPath, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL);
ULONGLONG GetFileSize(LPCTSTR szFilepath);
_tstring file_name(LPCTSTR szFilePath);
BOOL Write(LPCTSTR szFilepath, _tstring s);
BOOL WriteLn(LPCTSTR szFilepath, _tstring s);
void GetFileList(_tstring const& dir, _tstring const& mask, std::list<_tstring>& filelist);

} // namespace fileutils

#endif // fileutils_h__