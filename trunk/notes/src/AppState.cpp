#include "StdAfx.h"
#include "AppState.h"

CAppState::CRecentFile::CRecentFile(LPCTSTR name) : m_name(name)
{
}


CAppState::CAppState(void)
{
}

CAppState::~CAppState(void)
{
}

const CAppState::CRecentFile& CAppState::GetLastDataFile() const
{
	return *(m_recent_files.begin());
}

void CAppState::SetLastDataFile(LPCTSTR file_name)
{
	CRecentFileList::const_iterator it;
	for (it = m_recent_files.begin(); it != m_recent_files.end(); ++it)
	{
		if (lstrcmpi(it->GetName().c_str(), file_name) == 0)
			break;
	}
	if (it == m_recent_files.end())
	{
		m_recent_files.push_front(CRecentFile(file_name));
	}
	else if (it != m_recent_files.begin())
	{
		m_recent_files.splice(m_recent_files.begin(), m_recent_files, it);
	}
	while (m_recent_files.size() > 20)
		m_recent_files.pop_back();
}

const CAppState::CRecentFile& CAppState::GetDataFile(int index) const
{
	CRecentFileList::const_iterator it;
	int i = 0;
	for (it = m_recent_files.begin(); it != m_recent_files.end(); ++it)
	{
		if (i == index)
			break;
		++i;
	}
	if (it == m_recent_files.end())
	{
		ThrowError(_T("Индекс за пределами границ."));
	}
	return *it;
}