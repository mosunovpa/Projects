#pragma once

#include "note.h"

class CStorage
{
public:
	typedef std::vector<CNote> NotesList;
	typedef std::vector<int> NotesIdsList;

	CStorage(void);
	~CStorage(void);

	void SaveNote(CNote& note);
	void DeleteNote(int nNoteId);
	void GetAllNotes(NotesList& list) const;
	void GetAllNotesIds(NotesIdsList& list) const;
};
