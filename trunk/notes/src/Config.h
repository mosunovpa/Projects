#pragma once

class CConfig
{
public:
	CConfig(void);
	~CConfig(void);
	_tstring GetLastDataFileName() const;
	void SetLastDataFileName (_tstring const& file_name);
private:
	_tstring m_last_datafile_name;
};
