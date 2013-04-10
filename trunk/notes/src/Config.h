#pragma once

class CConfig
{
public:
	CConfig(void);
	~CConfig(void);
	_tstring GetLastDataFileName();
	void SetLastDataFileName (_tstring const& file_name);
};
