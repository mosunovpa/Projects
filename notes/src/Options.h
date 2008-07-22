#pragma once

class COptions
{
public:
	enum FontSize
	{
		FS_SMALL = 1,
		FS_MEDIUM,
		FS_LARGE
	};
	COptions(void);
	~COptions(void);

	BOOL GetAlwaysOnTop() const;
	void SetAlwaysOnTop(BOOL bVal);
	FontSize GetFontSize() const;
	void SetFontSize(FontSize fs);
private:
	BOOL m_bAlwaysOnTop;
	FontSize m_fs;

};
