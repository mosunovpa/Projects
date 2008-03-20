#pragma once

#include "note.h"

class CStorage
{
public:
	typedef std::vector<CNote> NotesList;

	CStorage(void);
	~CStorage(void);

	void SaveNote(CNote& note);
	void DeleteNote(int nNoteId);
	int GetNotesCount() const;
	void GetAllNotes(NotesList& list);
};
