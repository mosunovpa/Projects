#pragma once

#include "note.h"

class COptions;

class CStorage
{
public:
	CStorage(void);
	~CStorage(void);
	void Release();
	int SaveNote(CNote const& note, UINT nMask);
	void DeleteNote(int nNoteId);
	void GetAllNotes(CNote::List& list, UINT nMask) const;
	void GetLabels(std::list<_tstring>& list) const;
	CNote GetNote(int nNoteId) const;
	void ReadOptions(COptions& opt) const;
	void WriteOptions(COptions const& opt) const;

};
