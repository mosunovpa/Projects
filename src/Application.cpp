
#include "stdafx.h"
#include "Application.h"
#include "NoteWnd.h"
#include "atlwinmisc.h"

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
		if (!m_TrayWnd.Create(NULL/*HWND_MESSAGE*/, CRect(0, 0, 0, 0)/*CWindow::rcDefault*/, APP_NAME, 
			WS_POPUP  /*|WS_VISIBLE | WS_SYSMENU *//*| WS_MINIMIZEBOX*/))
		{
			ThrowError(_T("can not create app window"));
		}
	}
}

void CApplication::CreateNote()
{
	m_listNotes.push_back(new CNoteWnd());
	CNoteWnd* pWnd = m_listNotes.back();

	pWnd->Create(m_TrayWnd, CWindow::rcDefault, _T("NoteWnd"), 
		WS_POPUPWINDOW | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | 
		WS_THICKFRAME /*| WS_SYSMENU| WS_MINIMIZEBOX*/, WS_EX_NOPARENTNOTIFY | WS_EX_TOOLWINDOW /*WS_CAPTION | , WS_EX_TOOLWINDOW | WS_EX_APPWINDOW*/
		//  		WS_CAPTION | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU | 
		//  		WS_THICKFRAME | WS_OVERLAPPED /*| WS_MINIMIZEBOX | WS_MAXIMIZEBOX*/, 
		//  		WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_WINDOWEDGE | WS_EX_APPWINDOW
		);
	m_TrayWnd.ShowWindow(SW_SHOW);
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
		UpdateAppWindowPos(GetLastNotePoint());
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

void CApplication::OnNoteMoved( CNoteWnd* pWnd, CPoint pt )
{
	UpdateAppWindowPos(pt);
}

void CApplication::UpdateAppWindowPos( CPoint pt )
{
//	m_TrayWnd.MoveWindow(CRect(pt, pt), FALSE); // need if Tray window has WS_MINIMIZEBOX style
}

CPoint CApplication::GetLastNotePoint()
{
	CPoint pt;
	if (!m_listNotes.empty())
	{
		CNoteWnd* pLastNote = m_listNotes.back();
		CWindowRect wr(pLastNote->m_hWnd);
		pt = wr.TopLeft();
	}
	return pt;
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
BOOL CApplication::IsAllNotesOpened() const
{
	int nOpened = 0;
	for (std::list<CNoteWnd*>::const_iterator it = m_listNotes.begin();
		it != m_listNotes.end(); ++it)
	{
		if ((*it)->GetId() > 0)
		{
			++nOpened;
		}
	}
	return nOpened == m_storage.GetNotesCount();
}

/**/
void CApplication::DeleteNote( CNoteWnd* pWnd )
{
	m_storage.DeleteNote(pWnd->GetId());
}