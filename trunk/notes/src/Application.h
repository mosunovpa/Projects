
#pragma once

#include "simplesinglton.h"
#include "TrayWnd.h"
#include "Storage.h"
#include "Options.h"

class CNoteWnd;

/* CApplication */

class CApplication : public CSimpleSinglton<CApplication>
{
	friend class CSimpleSinglton<CApplication>;

public:
	void CreateAppWindow();
	void CreateNote();
	int SaveNote(CNoteWnd* pWnd);
	void DeleteNote(CNoteWnd* pWnd);
	void ShowAllNotes();
	void CloseAllNotes();
	int GetOpenedNotesCount() const;
	int GetHiddenNotesCount() const;
	void ActivateTopNote();
	LPCTSTR GetDataFileName();
	void OnNoteClosed(CNoteWnd* pWnd);
	COptions& GetOptions();
	void SaveOptions();

protected:
	CApplication();
	virtual ~CApplication();


private:
	CNoteWnd* FindNote(int nNoteId) const;
	void OpenNote(CNote const& note);
	CNoteWnd* CreateNoteWnd(CRect& rc);
	CRect CalcNewNoteRect();
	void GetAllNotesPositions(CStorage::NotesList& notes);
	void GetSomePossiblePositions(CRect const& center, std::vector<CRect>& poss);
	CRect GetOptimumPosition(std::vector<CRect> const& vPossiblePositions, 
		CStorage::NotesList const& notes);

	CTrayWnd m_TrayWnd;
	std::list<CNoteWnd*> m_listNotes;
	CStorage m_storage;
	CString m_sDataFile;
	COptions m_options;
};
