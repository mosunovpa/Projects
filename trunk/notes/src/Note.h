#pragma once

class CNote
{
public:
	typedef std::vector<CNote> List;

	CNote(void);
	~CNote(void);

	int GetId() const;
	_tstring GetText() const;
	int GetTextLen() const;
	CRect GetPos() const;
	_timeb GetCreatedDate() const;
	_timeb GetModifiedDate() const;
	_timeb GetDeletedDate() const;
	_tstring GetLabel() const;

	void SetId(int id);
	void SetText(_tstring const& text);
	void SetPos(CRect const& pos);
	void SetCreatedDate(_timeb dt);
	void SetModifiedDate(_timeb dt);
	void SetDeletedDate(_timeb dt);
	void SetLabel(LPCTSTR label);
private:
	int m_id;
	_tstring m_text;
	CRect m_pos;
	_timeb m_dtCreated;
	_timeb m_dtModified;
	_timeb m_dtDeleted;
	_tstring m_label;
};