
#pragma once

#include "simplesinglton.h"
#include "Storage.h"
#include "LocalStorage.h"
#include "Options.h"
#include "AppState.h"
#include "defines.h"

class CNoteWnd;
class CTrayWnd;
class CSettingsSheet;
class CExplorerWnd;

typedef void (CNoteWnd::* NotesProcessFunc)();

/* CApplication */

class CApplication : public CSimpleSinglton<CApplication>
{
	friend class CSimpleSinglton<CApplication>;
public:
	void CreateAppWindow();
	_tstring OpenNotebookDialog(HWND parent) const;
	void MoveToNotebook(int nNoteId, LPCTSTR sFileName);
	void OpenDataFile(LPCTSTR file_name);
	HWND CreateNote(_tstring const& sText = _tstring(), DWORD nFlag = NF_NONE);
	BOOL IsNoteVisible(int nNoteId) const;
	int SaveNote(CNoteWnd* pWnd, UINT nMask);
	int SaveNote(CNote const& note, UINT nMask);
	void DeleteFromStorage(int nNoteId, bool forever = false);
	void DeleteNote(int nNoteId, int forever = false);
	void RestoreNote(int nNoteId);
	BOOL IsNoteDeleted(int nNoteId);
	BOOL IsNoteExists(int nNoteId);
	void Command(int nCmd, int nNoteId);
	void Command(int nCmd, HWND hWnd);
	void ShowAllNotes();
	void ShowNote(int nNoteId);
	void CloseAllNotes(CNoteWnd* pExceptWnd = NULL);
	int GetOpenedNotesCount() const;
	int GetAllNotes(CNote::List& notes, UINT nMask);
	int GetHiddenNotesCount();
	void ActivateTopNote();
	COptions GetOptions();
	const CAppState& GetState() const { return m_state; }
	void SaveOptions(COptions const& opt);
	//void NoteTextToClipboard(int nNoteId);
	_tstring GetNoteCaption(_tstring const& text);
	void GetLabels(std::list<_tstring>& list);
	_tstring GetNoteLabel(int nNoteId);
	void SetNoteLabel(int nNoteId, LPCTSTR label);
	_tstring GetNoteText(int nNoteId);
	void DuplicateNote(int nNoteId);
	_tstring GetAppFolder() const;
	void EnumNoteWnds(NotesProcessFunc func);
	const _tstring & GetDataFileName() const;
	void ShowSettings();
	void ShowExplorer();

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

	std::auto_ptr<CTrayWnd> m_TrayWnd;
	std::auto_ptr<CSettingsSheet> m_pSettingsDlg;
	std::auto_ptr<CExplorerWnd> m_pExplorer;

	std::list<CNoteWnd*> m_listNotes;
	CLocalStorage m_local_storage;
	CAppState m_state;

	std::auto_ptr<CStorage> m_datafile;
};
