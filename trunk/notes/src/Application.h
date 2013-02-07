
#pragma once

#include "simplesinglton.h"
#include "TrayWnd.h"
#include "Storage.h"
#include "LocalStorage.h"
#include "Options.h"
#include "Config.h"
#include "defines.h"

class CNoteWnd;
typedef void (CNoteWnd::* NotesProcessFunc)();

class CDataFile
{
public:
	CDataFile(LPCTSTR file_name);
	COptions& GetOptions() { return m_options; }
	CStorage& GetStorage() { return m_datafile_storage; }
private:
	CStorage m_datafile_storage;
	COptions m_options;
};


/* CApplication */

class CApplication : public CSimpleSinglton<CApplication>
{
	friend class CSimpleSinglton<CApplication>;
public:
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
	int GetAllNotes(CNote::List& notes, UINT nMask) ;
	int GetHiddenNotesCount() ;
	void ActivateTopNote();
	LPCTSTR GetDataFileName();
	COptions& GetOptions();
	void SaveOptions();
	void NoteTextToClipboard(int nNoteId);
	_tstring GetNoteCaption(_tstring const& text);
	void GetLabels(std::list<_tstring>& list) ;
	_tstring GetNoteLabel(int nNoteId) ;
	void SetNoteLabel(int nNoteId, LPCTSTR label);
	_tstring GetNoteText(int nNoteId) ;
	void DuplicateNote(int nNoteId);
	_tstring GetAppFolder();
	void EnumNoteWnds(NotesProcessFunc func);

	// events
	void OnNoteClosed(CNoteWnd* pWnd);
	void OptionsUpdated();
	void OnAppClosed();

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
	CNote FindNote(int nNoteId) ;
	void UpdateNoteWnd( CNoteWnd* pWnd, CNote const& note );
	void OpenDataFile(LPCTSTR file_name);

	CTrayWnd m_TrayWnd;
	std::list<CNoteWnd*> m_listNotes;
	CLocalStorage m_local_storage;
	CConfig m_config;

	std::auto_ptr<CDataFile> m_datafile;
};
