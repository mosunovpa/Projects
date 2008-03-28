#pragma once

class COptions
{
public:
	COptions(void);
	~COptions(void);

	BOOL GetAlwaysOnTop() const;
	void SetAlwaysOnTop(BOOL bVal);
private:
	BOOL m_bAlwaysOnTop;
};
