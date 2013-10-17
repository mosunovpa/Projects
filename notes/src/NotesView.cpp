#include "stdafx.h"
#include "NotesView.h"
#include "Application.h"
#include "Clipboard.h"
#include "resource.h"

/**/
void CNotesView::PopulateLabelMenu(CMenuHandle menuLabels, _tstring const& sLabel, BOOL bCheckPadio /*= TRUE*/)
{
	m_listLabels.clear();
	CApplication::Get().GetLabels(m_listLabels);
	if (!m_listLabels.empty())
	{
		menuLabels.AppendMenu(MF_SEPARATOR);
	}
	int pos = 1;
	for (std::list<_tstring>::iterator it = m_listLabels.begin();
		it != m_listLabels.end(); ++it)
	{
		int nCmd = CREATE_LABEL_CMD(pos);
		menuLabels.AppendMenu(MF_STRING, nCmd, it->c_str());
		if (*it == sLabel)
		{
			menuLabels.CheckMenuRadioItem(LABEL_CMD_FIRST, LABEL_CMD_LAST, nCmd, MF_BYCOMMAND);
		}
		++pos;
	}
}

/**/
void CNotesView::ProcessSelectedNotes(const std::list<int>& notes, NotesActions action, LPCTSTR sParam /*= NULL*/)
{
	_tstring clipboartText;
	_tstring textSeparator = _T("\r\n---\r\n");

	std::list<int>::const_iterator it = notes.begin();
	for (; it != notes.end(); ++it)
	{
		int noteId = *it;
		switch (action)
		{
		case acOpen:
			CApplication::Get().ShowNote(noteId);
			break;
		case acDelete:
			CApplication::Get().DeleteNote(noteId);
			break;
		case acLabel:
			CApplication::Get().SetNoteLabel(noteId, sParam);
			break;
		case acClipboard:
			{
				_tstring s = CApplication::Get().GetNoteText(noteId);
				if (!clipboartText.empty()) 
				{
					clipboartText += textSeparator;
				}
				clipboartText += s;
			}
			break;
		case acRestore:
			CApplication::Get().RestoreNote(noteId);
			break;
		}
	}
	if (!clipboartText.empty())
	{
		CClipboard::SetText(clipboartText.c_str()/*, m_hWnd*/);
	}
}

/**/
_tstring CNotesView::GetLabelFromCmd(int nLabelCmd)
{
	_tstring sLabel;
	if (IS_LABEL_CMD(nLabelCmd))
	{
		int pos = GET_LABEL_ID_FROM_CMD(nLabelCmd);
		int i = 1;
		for (std::list<_tstring>::iterator it = m_listLabels.begin();
			it != m_listLabels.end(); ++it)
		{
			if (pos == i)
			{
				sLabel = it->c_str();
				break;
			}
			++i;
		}
	}
	return sLabel;
}

/**/
void CNotesView::PopulateNotebooksMenu(CMenuHandle menuNotebooks)
{
	const CAppState::CRecentFileList& rf =  CApplication::Get().GetState().GetRecentFiles();

	m_listNotebooks.clear();
	for (CAppState::CRecentFileList::const_iterator it = rf.begin(); it != rf.end(); ++it)
	{
		m_listNotebooks.push_back(it->GetName());
	}
	m_listNotebooks.sort();

	int pos = 0;
	for (std::list<_tstring>::iterator nit = m_listNotebooks.begin(); nit != m_listNotebooks.end(); ++nit)
	{
		int nCmd = CREATE_NOTEBOOK_CMD(pos);
		menuNotebooks.InsertMenu(pos, MF_BYPOSITION, nCmd, nit->c_str());
		if (CApplication::Get().GetDataFileName() == *nit)
		{
			menuNotebooks.CheckMenuRadioItem(NOTEBOOK_CMD_FIRST, NOTEBOOK_CMD_LAST, nCmd, MF_BYCOMMAND);
		}
		++pos;	
	}
}

/**/
_tstring CNotesView::GetNotebookFromCmd(int nCmd)
{
	_tstring sFile;
	if (IS_NOTEBOOK_CMD(nCmd))
	{
		int pos = GET_NOTEBOOK_ID_FROM_CMD(nCmd);
		int i = 0;
		for (std::list<_tstring>::iterator it = m_listNotebooks.begin();
			it != m_listNotebooks.end(); ++it)
		{
			if (pos == i)
			{
				sFile = it->c_str();
				break;
			}
			++i;
		}
	}
	return sFile;
}

/**/
void CNotesView::ProcessNotebookOpen(int nCmd)
{
	_tstring sFile;
	if (nCmd == ID_NOTEBOOK_OPEN)
	{
		sFile = CApplication::Get().OpenNotebookDialog();
	}
	else
	{
		sFile = GetNotebookFromCmd(nCmd);
	}

	if (!sFile.empty() && CApplication::Get().GetDataFileName() != sFile)
	{
		CApplication::Get().OpenDataFile(sFile.c_str());
	}
}

/**/
bool CNotesView::PopulateMoveToNotebooksMenu(CMenuHandle menuNotebooks)
{
	const CAppState::CRecentFileList& rf = CApplication::Get().GetState().GetRecentFiles();

	m_listMoveToNotebooks.clear();
	for (CAppState::CRecentFileList::const_iterator it = rf.begin(); it != rf.end(); ++it)
	{
		if (CApplication::Get().GetDataFileName() != it->GetName())
		{
			m_listMoveToNotebooks.push_back(it->GetName());
		}
	}
	m_listMoveToNotebooks.sort();

	int pos = 0;
	int nCmd = CREATE_MOVE_TO_NOTEBOOK_CMD(0);
	bool added = false;
	for (std::list<_tstring>::iterator it = m_listMoveToNotebooks.begin(); it != m_listMoveToNotebooks.end(); ++it)
	{
		menuNotebooks.InsertMenu(pos++, MF_BYPOSITION, nCmd++, it->c_str());
		added = true;
	}
	return added;
}

/**/
_tstring CNotesView::GetMoveToNotebookFromCmd(int nCmd)
{
	_tstring sFile;
	if (IS_MOVE_TO_NOTEBOOK_CMD(nCmd))
	{
		int pos = GET_MOVE_NOTEBOOK_ID_FROM_CMD(nCmd);
		int i = 0;
		for (std::list<_tstring>::iterator it = m_listMoveToNotebooks.begin();
			it != m_listMoveToNotebooks.end(); ++it)
		{
			if (pos == i)
			{
				sFile = it->c_str();
				break;
			}
			++i;
		}
	}
	return sFile;
}

/**/
void CNotesView::ProcessMoveToNotebook(int nCmd, int nNoteId)
{
	_tstring sFile;
	if (nCmd == ID_TNM_NOTEBOOK) // открыть блокнот для перемещения
	{
		sFile = CApplication::Get().OpenNotebookDialog();
	}
	else // переместить в указанный блокнот
	{
		sFile = GetMoveToNotebookFromCmd(nCmd);
	}
	if (!sFile.empty() && CApplication::Get().GetDataFileName() != sFile)
	{
		CApplication::Get().MoveToNotebook(sFile.c_str(), nNoteId);
	}
}