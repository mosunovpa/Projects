#pragma once

#include "note.h"

class COptions;

class CStorage
{
public:
	CStorage(void);
	~CStorage(void);
	void Release();
	void SaveNote(CNote& note);
	void DeleteNote(int nNoteId);
	void GetAllNotes(CNote::List& list, UINT nMask) const;
	CNote GetNote(int nNoteId) const;
	void ReadOptions(COptions& opt) const;
	void WriteOptions(COptions const& opt) const;

};
