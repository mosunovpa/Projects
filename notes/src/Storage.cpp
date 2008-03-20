#include "StdAfx.h"
#include "Storage.h"

CStorage::CStorage(void)
{
}

CStorage::~CStorage(void)
{
}

void CStorage::SaveNote(CNote& note)
{
	BOOL bIsEmpty = note.GetText().Trim().IsEmpty();

	if (note.GetId() == 0) // new note
	{
		if (!bIsEmpty)
		{
			NewNote(note);
		}
	}
	else
	{
		if (!bIsEmpty) // change note
		{
			UpdateNote(note);
		}
		else // note is empty - delete
		{
			DeleteNote(note.GetId());
		}
	}
}

int CStorage::GetNotesCount() const
{
	return 0;
}

void CStorage::GetAllNotes( CStorage::NotesList& list )
{

}

void CStorage::NewNote(CNote& note)
{
}

void CStorage::UpdateNote(CNote const& note)
{

}

void CStorage::DeleteNote(int nNoteId)
{

}