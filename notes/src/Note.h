#pragma once

class CNote
{
public:
	enum
	{
		GNM_ID = 0x01,
		GNM_TEXT = 0x02,
		GNM_POS = 0x4
	};
	typedef std::vector<CNote> List;

	CNote(void);
	~CNote(void);

	int GetId() const;
	CString GetText() const;
	CRect GetPos() const;

	void SetId(int id);
	void SetText(CString const& text);
	void SetPos(CRect const& pos);
private:
	int m_id;
	CString m_text;
	CRect m_pos;
};