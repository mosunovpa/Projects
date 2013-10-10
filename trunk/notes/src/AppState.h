#pragma once

class CAppState
{
public:
	class CRecentFile
	{
	public:
		CRecentFile(LPCTSTR name);
		_tstring const& GetName() const { return m_name; }
	private:
		_tstring m_name;
		std::list<int> m_recent_notes;
	};

	typedef std::list<CRecentFile> CRecentFileList;

	CAppState(void);
	~CAppState(void);
	const CRecentFile& GetLastDataFile() const;
	void SetLastDataFile (LPCTSTR file_name);
	const CRecentFile& GetDataFile(int index) const;
	const CRecentFileList & GetRecentFiles() const { return m_recent_files; }
private:
	CRecentFileList m_recent_files;
};
