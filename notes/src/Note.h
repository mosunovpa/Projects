#pragma once

class CNote
{
public:
	typedef std::vector<CNote> List;

	CNote(void);
	~CNote(void);

	int GetId() const;
	_tstring GetText() const;
	CRect GetPos() const;

	void SetId(int id);
	void SetText(LPCTSTR text);
	void SetPos(CRect const& pos);
private:
	int m_id;
	_tstring m_text;
	CRect m_pos;
};