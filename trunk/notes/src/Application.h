
#pragma once

#include "simplesinglton.h"
#include "TrayWnd.h"
#include "Storage.h"

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
	void RestoreFocus();
	void SetFocused(CNoteWnd* pWnd);
	LPCTSTR GetDataFileName();
	void OnNoteClosed(CNoteWnd* pWnd);

protected:
	CApplication();
	virtual ~CApplication();


private:
	CNoteWnd* FindNote(int nNoteId) const;
	void OpenNote(CNote const& note);
	CNoteWnd* CreateNoteWnd(CRect& rc);

	CTrayWnd m_TrayWnd;
	std::list<CNoteWnd*> m_listNotes;
	CNoteWnd* m_pFocused;
	CStorage m_storage;
	CString m_sDataFile;
};
