
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
	int IsAllNotesOpened() const;
	void RestoreFocus();
	void SetFocused(CNoteWnd* pWnd);

	void OnNoteMoved(CNoteWnd* pWnd, CPoint pt);
	void OnNoteClosed(CNoteWnd* pWnd);

protected:
	CApplication();
	virtual ~CApplication();
private:
	void UpdateAppWindowPos(CPoint pt);
	CPoint GetLastNotePoint();
	CTrayWnd m_TrayWnd;
	std::list<CNoteWnd*> m_listNotes;
	CNoteWnd* m_pFocused;
	CStorage m_storage;
};
