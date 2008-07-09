
#include "stdafx.h"
#include "resource.h"
#include "Application.h"
#include "NoteWnd.h"
#include "atlwinmisc.h"
#include "resutils.h"
#include "dateutils.h"
#include "fileutils.h"
#include "winutils.h"
#include <math.h>

/**/
CApplication::CApplication()
{
	m_storage.ReadOptions(m_options);
}

/**/
CApplication::~CApplication()
{
}

/**/
void CApplication::CreateAppWindow()
{
	if (!::IsWindow(m_TrayWnd.m_hWnd))
	{
		if (!m_TrayWnd.Create(NULL/*HWND_MESSAGE*/, CRect(0, 0, 0, 0)/*CWindow::rcDefault*/, RESSTR(IDS_APP_NAME), 
			WS_POPUP | WS_SYSMENU  /*|WS_VISIBLE *//*| WS_MINIMIZEBOX*/))
		{
			ThrowError(_T("can not create app window"));
		}
	}
}

/**/
void CApplication::GetAllNotesPositions(CNote::List& notes)
{
	m_storage.GetAllNotes(notes, CApplication::NM_POS);
	for (std::list<CNoteWnd*>::iterator it = m_listNotes.begin(); it != m_listNotes.end(); ++it)
	{
		if ((*it)->GetId() == 0) // new not saved note
		{
			CNote note;
			note.SetPos(CWindowRect((*it)->m_hWnd));
			notes.push_back(note);
		}
	}
}

/**/
void CApplication::GetSomePossiblePositions(CRect const& center, std::vector<CRect>& poss)
{
	const int nRadius = 150;
	for (int i = 0; i < 10; ++i) // 10 possible positions
	{
		CRect rc(center);
		int nRandAngle = ((double)rand() / (double)RAND_MAX) * 360;
		int dx = nRadius * cos((double)nRandAngle);
		int dy = nRadius * sin((double)nRandAngle);
		rc += CPoint(dx, dy);
		if (winutils::WinPosIsValid(rc))
		{
			poss.push_back(rc);
		}
	}
}

/**/
CRect CApplication::GetOptimumPosition(std::vector<CRect> const& vPossiblePositions, 
									   CNote::List const& notes)
{
	CRect rc;
	int nUnderTopLeftMin = 100000; // notes under top left corner on the new note
	int nOverlappedTopLeftMin = 100000; // top left corners under new note area
	for (int i = 0; i < vPossiblePositions.size(); ++i)
	{
		int nUnderTopLeft = 0;
		int nOverlappedTopLeft = 0;
		for (int j = 0; j < notes.size(); ++j)
		{
			if (notes[j].GetPos().PtInRect(vPossiblePositions[i].TopLeft()))
			{
				++nUnderTopLeft;
			}
			if (vPossiblePositions[i].PtInRect(notes[j].GetPos().TopLeft()))
			{
				++nOverlappedTopLeft;
			}
		}
		if (nUnderTopLeft < nUnderTopLeftMin)
		{
			nUnderTopLeftMin = nUnderTopLeft;
			nOverlappedTopLeftMin = nOverlappedTopLeft;
			rc = vPossiblePositions[i];
		}
		else if (nUnderTopLeft == nUnderTopLeftMin && nOverlappedTopLeft < nOverlappedTopLeftMin)
		{
			nOverlappedTopLeftMin = nOverlappedTopLeft;
			rc = vPossiblePositions[i];
		}
	}
	return rc;
}

/**/
CRect CApplication::CalcNewNoteRect()
{
	const int nNewNoteWidth = 200;
	const int nNewNoteHeight = 160;

	// collect all notes positions
	CNote::List notes;
	GetAllNotesPositions(notes);

	// if first note return center point
	if (!notes.empty())
	{
		// get some possible position
		std::vector<CRect> vPossiblePositions;
		CRect rcLast = notes.back().GetPos();
		GetSomePossiblePositions(rcLast, vPossiblePositions);

		// select minimum overlapped window
		CRect rc(GetOptimumPosition(vPossiblePositions, notes));
		if (!rc.IsRectNull())
		{
			return rc;
		}
	}

	// default value
	CRect war;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &war, 0);
	CPoint center(war.right / 2, war.bottom / 2);
	return CRect(center.x, center.y, center.x + nNewNoteWidth, center.y + nNewNoteHeight);
}

/**/
void CApplication::CreateNote()
{
	CNoteWnd* pWnd = CreateNoteWnd(CalcNewNoteRect());
	if (pWnd)
	{
		pWnd->SetCreated(dateutils::GetCurrentDate());
		pWnd->SetFocus();
	}
}

/**/
void CApplication::CloseAllNotes(CNoteWnd* pExceptWnd /*= NULL*/)
{
	std::vector<HWND> listNotes;
	// copy to other list for deletion
	for (std::list<CNoteWnd*>::reverse_iterator it = m_listNotes.rbegin();
		it != m_listNotes.rend(); ++it)
	{
		if (pExceptWnd != (*it))
		{
			listNotes.push_back((*it)->m_hWnd);
		}
	}
	for (int i = 0; i < listNotes.size(); ++i)
	{
		if (pExceptWnd == NULL || pExceptWnd->m_hWnd != listNotes[i])
		{
			::DestroyWindow(listNotes[i]); // OnNoteClosed will be invoked in CNoteWnd::OnFinalMessage
		}
	}
	ATLASSERT((pExceptWnd == NULL && m_listNotes.empty()) || 
		(pExceptWnd != NULL && m_listNotes.size() == 1));
}

/**/
void CApplication::ShowAllNotes()
{
	CNote::List list;
	m_storage.GetAllNotes(list, CApplication::NM_ALL);
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

/**/
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
		SetActiveWindow(NULL); // activate previous application in z-order
	}
	else
	{
		ActivateTopNote();
	}
}

/**/
void CApplication::ActivateTopNote()
{
	::SetFocus(winutils::GetTopWnd(NOTE_WND_CLASS_NAME));
}

/**/
int CApplication::SaveNote(CNoteWnd* pWnd, UINT nMask)
{
	CNote note;
	note.SetId(pWnd->GetId());
	note.SetText(pWnd->GetText().c_str());
	note.SetPos(CWindowRect(pWnd->m_hWnd));
	note.SetCreated(pWnd->GetCreated());
	m_storage.SaveNote(note, nMask);
	return note.GetId();
}

/**/
int CApplication::GetOpenedNotesCount() const
{
	return m_listNotes.size();
}

/**/
int CApplication::GetAllNotes(CNote::List& notes, UINT nMask) const
{
	m_storage.GetAllNotes(notes, nMask);
	return notes.size();
}

/**/
int CApplication::GetHiddenNotesCount() const
{
	CNote::List notes;
	m_storage.GetAllNotes(notes, CApplication::NM_ID);
	int nCount = 0;
	for (int i = 0; i < notes.size(); ++i)
	{
		if (!FindNote(notes[i].GetId()))
		{
			++nCount;
		}
	}
	return nCount;
}

/**/
void CApplication::DeleteNote(int nNoteId)
{
	m_storage.DeleteNote(nNoteId);
}

/**/
LPCTSTR CApplication::GetDataFileName()
{
	if (m_sDataFile.empty())
	{
		m_sDataFile.resize(MAX_PATH);
		LPTSTR szFile = &m_sDataFile[0];
		::SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_DEFAULT, szFile);
		::PathCombine(szFile, szFile, RESSTR(IDS_APP_NAME));
		if (!::PathFileExists(szFile))
		{
			fileutils::CreateDirectoryRecursive(szFile);
		}
		::PathCombine(szFile, szFile, _T("notes.dat"));
	}
	return m_sDataFile.c_str();
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

CNoteWnd* CApplication::OpenNote( CNote const& note )
{
	CNoteWnd* pWnd = CreateNoteWnd(note.GetPos());
	if (pWnd)
	{
		pWnd->SetId(note.GetId());
		pWnd->SetText(note.GetText());
		pWnd->SetCreated(note.GetCreated());
	}
	return pWnd;
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

COptions& CApplication::GetOptions()
{
	return m_options;
}

void CApplication::SaveOptions()
{
	m_storage.WriteOptions(m_options);
}

void CApplication::ShowNote(int nNoteId)
{
	CNoteWnd* pNoteWnd = FindNote(nNoteId);
	if (pNoteWnd)
	{
		pNoteWnd->SetFocus();
	}
	else
	{
		OpenNote(m_storage.GetNote(nNoteId));
	}
}

void CApplication::NoteTextToClipboard(int nNoteId)
{
	CNoteWnd* pNoteWnd = FindNote(nNoteId);
	if (!pNoteWnd)
	{
		pNoteWnd = OpenNote(m_storage.GetNote(nNoteId));
		pNoteWnd->PostMessage(WM_COMMAND, ID_CLIPBRD_COPY);
		pNoteWnd->PostMessage(WM_CLOSE);
	}
	else
	{
		pNoteWnd->PostMessage(WM_COMMAND, ID_CLIPBRD_COPY);
	}
}

void CApplication::ReleaseStorage()
{
	m_storage.Release();
}