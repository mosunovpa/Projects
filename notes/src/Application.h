
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
	enum NoteMask
	{
		NM_NONE = 0,
		NM_ID = 1,
		NM_TEXT = 2,
		NM_POS = 4,
		NM_CREATED = 8,
		NM_MODIFIED = 16,
		NM_DELETED = 32,
		NM_LABEL = 64,
		NM_ALL = NM_ID 
					| NM_TEXT 
					| NM_POS 
					| NM_CREATED 
					| NM_MODIFIED 
					| NM_DELETED 
					| NM_LABEL
	};

	enum NoteFlags
	{
		NF_NONE = 0,
		NF_ROLLUP = 1,
		NF_NOACTIVATE = 2
	};

	void CreateAppWindow();
	HWND CreateNote(_tstring const& sText = _tstring(), DWORD nFlag = NF_NONE);
	BOOL IsNoteVisible(int nNoteId) const;
	int SaveNote(CNoteWnd* pWnd, UINT nMask);
	int SaveNote(CNote const& note, UINT nMask);
	void DeleteFromStorage(int nNoteId);
	void DeleteNote(int nNoteId);
	void RestoreNote(int nNoteId);
	BOOL IsNoteDeleted(int nNoteId);
	BOOL IsNoteExists(int nNoteId);
	void Command(int nCmd, int nNoteId);
	void Command(int nCmd, HWND hWnd);
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
	void ReleaseStorage();
	void NoteTextToClipboard(int nNoteId);
	_tstring GetNoteCaption(_tstring text);
	void OptionsUpdated();
	void GetLabels(std::list<_tstring>& list) const;
	_tstring GetNoteLabel(int nNoteId) const;
	void SetNoteLabel(int nNoteId, _tstring const& sLabel);
	_tstring GetNoteText(int nNoteId) const;
	void DuplicateNote(int nNoteId);
protected:
	CApplication();
	virtual ~CApplication();


private:
	CNoteWnd* OpenNote(CNote const& note);
	CNoteWnd* CreateNoteWnd(CRect& rc);
	CNoteWnd* FindNoteWnd(int nNoteId) const;
	void DeleteNoteWnd(CNoteWnd* pNoteWnd);
	CRect CalcNewNoteRect();
	void GetAllNotesPositions(CNote::List& notes);
	void GetSomePossiblePositions(CRect const& center, std::vector<CRect>& poss);
	CRect GetOptimumPosition(std::vector<CRect> const& vPossiblePositions, 
		CNote::List const& notes);
	CNote FindNote(int nNoteId) const;
	void UpdateNoteWnd( CNoteWnd* pWnd, CNote const& note );

	CTrayWnd m_TrayWnd;
	std::list<CNoteWnd*> m_listNotes;
	CStorage m_storage;
	_tstring m_sDataFile;
	COptions m_options;
};
