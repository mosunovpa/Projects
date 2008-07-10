#pragma once

class CNote
{
public:
	typedef std::vector<CNote> List;

	CNote(void);
	~CNote(void);

	int GetId() const;
	LPCTSTR GetText() const;
	int GetTextLen() const;
	CRect GetPos() const;
	time_t GetCreated() const;

	void SetId(int id);
	void SetText(LPCTSTR text);
	void SetPos(CRect const& pos);
	void SetCreated(time_t dt);
private:
	int m_id;
	_tstring m_text;
	CRect m_pos;
	time_t m_dtCreated;
};