
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
	// GetAllNotes mask
	enum
	{
		GNM_ID = 0x01,
		GNM_TEXT = 0x02,
		GNM_POS = 0x4,
		GNM_ALL = GNM_ID | GNM_TEXT | GNM_POS
	};

	void CreateAppWindow();
	void CreateNote();
	int SaveNote(CNoteWnd* pWnd);
	void DeleteNote(CNoteWnd* pWnd);
	void ShowAllNotes();
	void ShowNote(int nNoteId);
	void CloseAllNotes(CNoteWnd* pExceptWnd = NULL);
	int GetOpenedNotesCount() const;
	int GetAllNotes(CNote::List& notes, UINT nMask) const;
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
	void GetAllNotesPositions(CNote::List& notes);
	void GetSomePossiblePositions(CRect const& center, std::vector<CRect>& poss);
	CRect GetOptimumPosition(std::vector<CRect> const& vPossiblePositions, 
		CNote::List const& notes);

	CTrayWnd m_TrayWnd;
	std::list<CNoteWnd*> m_listNotes;
	CStorage m_storage;
	_tstring m_sDataFile;
	COptions m_options;
};
