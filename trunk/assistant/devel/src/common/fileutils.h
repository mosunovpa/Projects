/**
 * \file fileutils.h
 *
 * \since 2006/03/28
 * \author pavel
 */
#ifndef fileutils_h__
#define fileutils_h__

#include "wstring.h"

namespace fileutils
{
BOOL CreateDirectoryRecursive(LPCTSTR szPath, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL);
ULONGLONG GetFileSize(LPCTSTR szFilepath);
_tstring file_name(LPCTSTR szFilePath);

} // namespace fileutils

#endif // fileutils_h__