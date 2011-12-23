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
	SIZE GetNewNoteSize() const;
	void SetNewNoteSize(SIZE const& newnote_size);
private:
	BOOL m_bAlwaysOnTop;
	FontSize m_fs;
	CSize m_newnote_size;
};
