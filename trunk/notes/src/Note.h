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
	time_t GetCreatedDate() const;
	time_t GetModifiedDate() const;
	time_t GetDeletedDate() const;

	void SetId(int id);
	void SetText(LPCTSTR text);
	void SetPos(CRect const& pos);
	void SetCreatedDate(time_t dt);
	void SetModifiedDate(time_t dt);
	void SetDeletedDate(time_t dt);
private:
	int m_id;
	_tstring m_text;
	CRect m_pos;
	time_t m_dtCreated;
	time_t m_dtModified;
	time_t m_dtDeleted;
};