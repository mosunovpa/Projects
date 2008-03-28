#pragma once

#include "note.h"

class COptions;

class CStorage
{
public:
	enum
	{
		GNM_ID = 0x01,
		GNM_TEXT = 0x02,
		GNM_POS = 0x4
	};
	typedef std::vector<CNote> NotesList;

	CStorage(void);
	~CStorage(void);

	void SaveNote(CNote& note);
	void DeleteNote(int nNoteId);
	void GetAllNotes(NotesList& list, UINT nMask) const;
	void ReadOptions(COptions& opt) const;
	void WriteOptions(COptions const& opt) const;
};
