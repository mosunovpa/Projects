
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
#include "Clipboard.h"

/**/
CApplication::CApplication()
{
	_tstring sDataFile = GetDataFileFolder();
	if (!::PathFileExists(sDataFile.c_str()))
	{
		fileutils::CreateDirectoryRecursive(sDataFile.c_str());
	}
	TCHAR destBuf[MAX_PATH] = _T("");
	::PathCombine(destBuf, sDataFile.c_str(), _T("notes.dat"));

	m_sDataFile = destBuf;
	m_storage.SetDataFile(m_sDataFile.c_str());
	m_storage.ReadOptions(m_options);

}

/**/
CApplication::~CApplication()
{
}

/**/
_tstring CApplication::GetDataFileFolder()
{
	TCHAR destBuf[MAX_PATH] = _T("");
	TCHAR pathBuf[MAX_PATH] = _T("");
	::SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_DEFAULT, pathBuf);
	::PathCombine(destBuf, pathBuf, RESSTR(IDS_APP_NAME));
	return _tstring(destBuf);
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
	m_storage.GetAllNotes(notes, NM_POS);
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
	const int nRadius = 100;
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

	if (vPossiblePositions.size() > 0)
	{
		int pos = floor(((double) rand() / 
			(double) RAND_MAX) * vPossiblePositions.size());
		if (pos < vPossiblePositions.size())
			return vPossiblePositions[pos];
	}

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
HWND CApplication::CreateNote(_tstring const& sText /*= _tstring()*/, DWORD nFlag /*= NF_NONE*/)
{
	CNoteWnd* pWnd = CreateNoteWnd(CalcNewNoteRect());
	if (pWnd)
	{
		pWnd->SetInitFlags(nFlag);
		pWnd->SetCreatedDate(dateutils::GetCurrentDate());
		pWnd->SetFocus();

		if (!sText.empty())
		{
			pWnd->SetText(sText);
		}
		if ( nFlag & NF_ROLLUP )
		{
			pWnd->Rollup();
		}
	}
	return pWnd != NULL ? pWnd->m_hWnd : NULL;
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
	m_storage.GetAllNotes(list, NM_ALL);
	for (int i = 0; i < list.size(); ++i)
	{
		CNoteWnd* pNoteWnd = FindNoteWnd(list[i].GetId());
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
	DeleteNoteWnd(pWnd);

	if (m_listNotes.empty())
	{
// 		if (::IsWindow(m_TrayWnd.m_hWnd))
// 		{
// 			m_TrayWnd.ShowWindow(SW_HIDE);
// 		}
//		SetActiveWindow(NULL); // activate previous application in z-order
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
	note.SetText(pWnd->GetText());
	note.SetPos(pWnd->GetRealNoteRect() /*CWindowRect(pWnd->m_hWnd)*/);
	note.SetCreatedDate(pWnd->GetCreatedDate());
	if (nMask & NM_MODIFIED)
	{
		note.SetModifiedDate(dateutils::GetCurrentDate());
	}
	note.SetDeletedDate(pWnd->GetDeletedDate());
	note.SetLabel(pWnd->GetLabel());
	return m_storage.SaveNote(note, nMask);
}

/**/
int CApplication::SaveNote(CNote const& note, UINT nMask)
{
	CNote nt = note;
	if (nMask & NM_MODIFIED)
	{
		nt.SetModifiedDate(dateutils::GetCurrentDate());
	}
	return m_storage.SaveNote(nt, nMask);
}

/**/
int CApplication::GetOpenedNotesCount() const
{
	return m_listNotes.size();
}

/**/
int CApplication::GetAllNotes(CNote::List& notes, UINT nMask) 
{
	m_storage.GetAllNotes(notes, nMask);
	return notes.size();
}

/**/
int CApplication::GetHiddenNotesCount() 
{
	CNote::List notes;
	m_storage.GetAllNotes(notes, NM_ID);
	int nCount = 0;
	for (int i = 0; i < notes.size(); ++i)
	{
		if (!FindNoteWnd(notes[i].GetId()))
		{
			++nCount;
		}
	}
	return nCount;
}

/**/
void CApplication::DeleteFromStorage(int nNoteId)
{
	if (nNoteId > 0)
	{
		CNote note = m_storage.GetNote(nNoteId);
//		note.SetLabel(_tstring());
		if (note.GetDeletedDate() == 0)
		{
			note.SetDeletedDate(dateutils::GetCurrentDate());
			note.SetModifiedDate(dateutils::GetCurrentDate());
			m_storage.SaveNote(note, NM_DELETED | NM_LABEL | NM_MODIFIED);
		}
		else
		{
			m_storage.DeleteNote(nNoteId);
		}
	}
}


/**/
CNoteWnd* CApplication::FindNoteWnd(int nNoteId) const
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

/**/
CNote CApplication::FindNote(int nNoteId) 
{
	return m_storage.GetNote(nNoteId);
}

/**/
void CApplication::UpdateNoteWnd( CNoteWnd* pWnd, CNote const& note )
{
	pWnd->SetId(note.GetId());
	pWnd->SetText(note.GetText());
	pWnd->SetCreatedDate(note.GetCreatedDate());
	pWnd->SetDeletedDate(note.GetDeletedDate());
	pWnd->SetLabel(note.GetLabel());
	pWnd->Refresh();
}

/**/
CNoteWnd* CApplication::OpenNote( CNote const& note )
{
	CNoteWnd* pWnd = CreateNoteWnd(note.GetPos());
	if (pWnd)
	{
		UpdateNoteWnd(pWnd, note);
	}
	return pWnd;
}

CNoteWnd* CApplication::CreateNoteWnd(CRect& rc)
{
	m_listNotes.push_back(new CNoteWnd());
	CNoteWnd* pWnd = m_listNotes.back();

	winutils::AdjustScreenRect(rc);
	pWnd->Create(m_TrayWnd, rc, _T("NoteWnd"), 
//		WS_OVERLAPPEDWINDOW | WS_VISIBLE ,
		/*WS_OVERLAPPED */ WS_POPUPWINDOW | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_THICKFRAME /*| WS_SYSMENU| WS_MINIMIZEBOX*/,
		WS_EX_NOPARENTNOTIFY | WS_EX_TOOLWINDOW /*| WS_EX_NOACTIVATE WS_CAPTION | , WS_EX_TOOLWINDOW | WS_EX_APPWINDOW*/
		//  		WS_CAPTION | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU | 
		//  		WS_THICKFRAME | WS_OVERLAPPED /*| WS_MINIMIZEBOX | WS_MAXIMIZEBOX*/, 
		//  		WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_WINDOWEDGE | WS_EX_APPWINDOW
		);
//	m_TrayWnd.ShowWindow(SW_SHOW);
	return pWnd;
}

/**/
void CApplication::DeleteNoteWnd(CNoteWnd* pNoteWnd)
{
	std::list<CNoteWnd*>::iterator it = std::find(m_listNotes.begin(), m_listNotes.end(), pNoteWnd);
	if (it != m_listNotes.end())
	{
		delete *it;
		m_listNotes.erase(it);
	}
}

/**/
COptions& CApplication::GetOptions()
{
	return m_options;
}

/**/
void CApplication::SaveOptions()
{
	m_storage.WriteOptions(m_options);
}

/**/
void CApplication::ShowNote(int nNoteId)
{
	CNoteWnd* pNoteWnd = FindNoteWnd(nNoteId);
	if (pNoteWnd)
	{
		CNote note = m_storage.GetNote(nNoteId);
		UpdateNoteWnd(pNoteWnd, note);
		pNoteWnd->Unroll();
		pNoteWnd->SetFocus();
	}
	else
	{
		OpenNote(m_storage.GetNote(nNoteId));
	}
}

/**/
void CApplication::DuplicateNote(int nNoteId)
{
	CNoteWnd* pWnd = CreateNoteWnd(CalcNewNoteRect());
	if (pWnd && nNoteId)
	{
		CNote note = m_storage.GetNote(nNoteId);
		note.SetId(0);
		UpdateNoteWnd(pWnd, note);
		pWnd->Unroll();
		pWnd->SetFocus();
	}
}

/**/
void CApplication::NoteTextToClipboard(int nNoteId)
{
	CNoteWnd* pNoteWnd = FindNoteWnd(nNoteId);
	if (pNoteWnd)
	{
		pNoteWnd->PostMessage(WM_COMMAND, ID_CLIPBRD_COPY);
	}
	else
	{
		CNote note = m_storage.GetNote(nNoteId);
		CClipboard::SetText(note.GetText().c_str(), m_TrayWnd);
	}
}

/**/
void CApplication::DeleteNote(int nNoteId)
{
	CNoteWnd* pNoteWnd = FindNoteWnd(nNoteId);
	if (pNoteWnd)
	{
		pNoteWnd->PostMessage(WM_COMMAND, ID_DELETE);
	}
	else
	{
		DeleteFromStorage(nNoteId);
	}
}

/**/
void CApplication::RestoreNote(int nNoteId)
{
	if (nNoteId > 0)
	{
		CNote note = m_storage.GetNote(nNoteId);
		note.SetDeletedDate(0);
		note.SetModifiedDate(dateutils::GetCurrentDate());
//		note.SetLabel(_tstring());
		m_storage.SaveNote(note, NM_DELETED | NM_MODIFIED | NM_LABEL);
		ShowNote(nNoteId);
	}
}

/**/
void CApplication::ReleaseStorage()
{
	m_storage.Release();
}

/**/
void CApplication::Command(int nCmd, int nNoteId)
{
	CNoteWnd* pNoteWnd = FindNoteWnd(nNoteId);
	if (!pNoteWnd)
	{
		pNoteWnd = OpenNote(m_storage.GetNote(nNoteId));
	}
	pNoteWnd->PostMessage(WM_COMMAND, nCmd);
}

/**/
void CApplication::Command(int nCmd, HWND hWnd)
{
	::PostMessage(hWnd, WM_COMMAND, nCmd, 0);
}

/**/
BOOL CApplication::IsNoteDeleted( int nNoteId )
{
	CNote note = m_storage.GetNote(nNoteId);
	return (note.GetDeletedDate() == 0);
}

/**/
BOOL CApplication::IsNoteExists( int nNoteId )
{
	try
	{
		CNote note = m_storage.GetNote(nNoteId);
		return TRUE;
	}
	catch(...)
	{
		return FALSE;
	}
}


/**/
_tstring CApplication::GetNoteCaption(_tstring text)
{
	_tstring sCaption = strutils::trim_string(text.c_str(), 64);
	std::replace(sCaption.begin(), sCaption.end(), _T('\t'), _T(' '));
	if (sCaption.size() > 0 && sCaption.size() < text.size())
	{
		sCaption += _T("...");
	}
	return sCaption;
}

/* */
void CApplication::OptionsUpdated()
{
	for (std::list<CNoteWnd*>::const_iterator it = m_listNotes.begin();
		it != m_listNotes.end(); ++it)
	{
		(*it)->OptionsUpdated();
	}
}

/**/
void CApplication::GetLabels(std::list<_tstring>& list) 
{
	m_storage.GetLabels(list);
}

/**/
BOOL CApplication::IsNoteVisible(int nNoteId) const
{
	return FindNoteWnd(nNoteId) != NULL;
}

_tstring CApplication::GetNoteLabel(int nNoteId) 
{
	CNote note = FindNote(nNoteId);
	return note.GetLabel();
}

void CApplication::SetNoteLabel(int nNoteId, _tstring const& sLabel)
{
	CNote note = FindNote(nNoteId);
	note.SetLabel(sLabel);
	note.SetModifiedDate(dateutils::GetCurrentDate());
	CApplication::Get().SaveNote(note, NM_LABEL | NM_MODIFIED);

	CNoteWnd* pWnd = FindNoteWnd(nNoteId);
	if (pWnd)
	{
		pWnd->SetLabel(sLabel);
	}
}

_tstring CApplication::GetNoteText(int nNoteId) 
{
	CNote note = FindNote(nNoteId);
	return note.GetText();
}

