
#include "stdafx.h"
#include "resource.h"
#include "Application.h"
#include "NoteWnd.h"
#include "atlwinmisc.h"
#include "resutils.h"
#include "fileutils.h"

CApplication::CApplication() : m_pFocused(NULL)
{
	
}

CApplication::~CApplication()
{
}

void CApplication::CreateAppWindow()
{
	if (!::IsWindow(m_TrayWnd.m_hWnd))
	{
		if (!m_TrayWnd.Create(NULL/*HWND_MESSAGE*/, CRect(0, 0, 0, 0)/*CWindow::rcDefault*/, RESSTR(IDS_APP_NAME), 
			WS_POPUP  /*|WS_VISIBLE | WS_SYSMENU *//*| WS_MINIMIZEBOX*/))
		{
			ThrowError(_T("can not create app window"));
		}
	}
}

void CApplication::CreateNote()
{
	CRect rc(0, 0, 200, 165 );
	CNoteWnd* pWnd = CreateNoteWnd(rc);
	if (pWnd)
	{
		pWnd->SetFocus();
	}
}

void CApplication::CloseAllNotes()
{
	std::vector<HWND> listNotes;
	for (std::list<CNoteWnd*>::reverse_iterator it = m_listNotes.rbegin();
		it != m_listNotes.rend(); ++it)
	{
		listNotes.push_back((*it)->m_hWnd);
	}
	for (int i = 0; i < listNotes.size(); ++i)
	{
		::DestroyWindow(listNotes[i]); // OnNoteClosed will be invoked in CNoteWnd::OnFinalMessage
	}
	ATLASSERT(m_listNotes.empty());
}

void CApplication::ShowAllNotes()
{
	CStorage::NotesList list;
	m_storage.GetAllNotes(list);
	for (int i = 0; i < list.size(); ++i)
	{
		CNoteWnd* pNoteWnd = FindNote(list[i].GetId());
		if (pNoteWnd)
		{
			
		}
		else
		{
			OpenNote(list[i]);
		}
	}
}

void CApplication::OnNoteClosed(CNoteWnd* pWnd)
{
	std::list<CNoteWnd*>::iterator it = std::find(m_listNotes.begin(), m_listNotes.end(), pWnd);
	if (it != m_listNotes.end())
	{
		delete *it;
		m_listNotes.erase(it);
	}
	if (m_listNotes.empty())
	{
		if (::IsWindow(m_TrayWnd.m_hWnd))
		{
			m_TrayWnd.ShowWindow(SW_HIDE);
		}
		SetFocused(NULL);
		SetActiveWindow(NULL); // activate previous application in z-order
	}
	else
	{
		SetFocused(m_listNotes.back());
		RestoreFocus();
	}
}

void CApplication::RestoreFocus()
{
	std::list<CNoteWnd*>::iterator it = std::find(m_listNotes.begin(), m_listNotes.end(), m_pFocused);
	if (it != m_listNotes.end() && ::IsWindow((*it)->m_hWnd))
	{
		(*it)->SetFocus();
	}
	else
	{
		SetFocused(NULL);
	}
}

void CApplication::SetFocused( CNoteWnd* pWnd )
{
	m_pFocused = pWnd;
}

/**/
int CApplication::SaveNote(CNoteWnd* pWnd)
{
	CNote note;
	note.SetId(pWnd->GetId());
	note.SetText(pWnd->GetText());
	note.SetPos(CWindowRect(pWnd->m_hWnd));
	m_storage.SaveNote(note);
	return note.GetId();
}

/**/
int CApplication::GetOpenedNotesCount() const
{
	return m_listNotes.size();
}

/**/
int CApplication::GetHiddenNotesCount() const
{
	CStorage::NotesIdsList list;
	m_storage.GetAllNotesIds(list);
	int nCount = 0;
	for (int i = 0; i < list.size(); ++i)
	{
		if (!FindNote(list[i]))
		{
			++nCount;
		}
	}
	return nCount;
}

/**/
void CApplication::DeleteNote( CNoteWnd* pWnd )
{
	m_storage.DeleteNote(pWnd->GetId());
}

/**/
LPCTSTR CApplication::GetDataFileName()
{
	if (m_sDataFile.IsEmpty())
	{
		LPTSTR szFile = m_sDataFile.GetBufferSetLength(MAX_PATH);
		::SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_DEFAULT, szFile);
		::PathCombine(szFile, szFile, RESSTR(IDS_APP_NAME));
		if (!::PathFileExists(szFile))
		{
			fileutils::CreateDirectoryRecursive(szFile);
		}
		::PathCombine(szFile, szFile, _T("notes.dat"));
		m_sDataFile.ReleaseBuffer();
	}
	return m_sDataFile;
}

CNoteWnd* CApplication::FindNote(int nNoteId) const
{
	for (std::list<CNoteWnd*>::const_iterator it = m_listNotes.begin();
		it != m_listNotes.end(); ++it)
	{
		if ((*it)->GetId() == nNoteId)
		{
			return (*it);
		}
	}
	return NULL;
}

void CApplication::OpenNote( CNote const& note )
{
	CNoteWnd* pWnd = CreateNoteWnd(note.GetPos());
	if (pWnd)
	{
		pWnd->SetId(note.GetId());
		pWnd->SetText(note.GetText());
		pWnd->SetFocus();
	}
}

CNoteWnd* CApplication::CreateNoteWnd(CRect& rc)
{
	m_listNotes.push_back(new CNoteWnd());
	CNoteWnd* pWnd = m_listNotes.back();

	pWnd->Create(m_TrayWnd, rc, _T("NoteWnd"), 
		WS_POPUPWINDOW | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | 
		WS_THICKFRAME /*| WS_SYSMENU| WS_MINIMIZEBOX*/, WS_EX_NOPARENTNOTIFY | WS_EX_TOOLWINDOW /*WS_CAPTION | , WS_EX_TOOLWINDOW | WS_EX_APPWINDOW*/
		//  		WS_CAPTION | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU | 
		//  		WS_THICKFRAME | WS_OVERLAPPED /*| WS_MINIMIZEBOX | WS_MAXIMIZEBOX*/, 
		//  		WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_WINDOWEDGE | WS_EX_APPWINDOW
		);
	m_TrayWnd.ShowWindow(SW_SHOW);
	return pWnd;
}