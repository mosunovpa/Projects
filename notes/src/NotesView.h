#pragma once

#include "Application.h"
#include "Clipboard.h"
#include "resource.h"


template<class T>
class CNotesView
{
public:
	enum NotesActions { acNone = 0, acDelete, acOpen, acLabel, acClipboard, acRestore, acMoveToNotebook };

	CNotesView(void) {};
	~CNotesView(void) {};


	/**/
	void PopulateLabelMenu(CMenuHandle menuLabels, _tstring const& sLabel)
	{
		m_listLabels.clear();
		CApplication::Get().GetLabels(m_listLabels);
		if (!sLabel.empty())
		{
			std::list<_tstring>::iterator it = std::find(m_listLabels.begin(), m_listLabels.end(), sLabel);
			if (it == m_listLabels.end())
			{
				m_listLabels.push_back(sLabel);
				m_listLabels.sort();
			}
		}
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
	void ProcessSelectedNotes(const std::list<int>& notes, NotesActions action, LPCTSTR sParam = NULL)
	{
		_tstring clipboartText;
		_tstring textSeparator = _T("\r\n---\r\n");

		std::list<int>::const_iterator it = notes.begin();
		for (; it != notes.end(); ++it)
		{
			Sleep(1); // пауза в 1 милисек, чтобы времена модификации (или удаления) заметок отличались 
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
			case acMoveToNotebook:
				CApplication::Get().MoveToNotebook(noteId, sParam);
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
	void ProcessSelectedNotes(NotesActions action, LPCTSTR sParam = NULL)
	{
		std::list<int> notes;
		T* pT = static_cast<T*>(this);
		pT->GetSelectedNotes(notes);
		ProcessSelectedNotes(notes, action, sParam);
	}

	/**/
	_tstring GetLabelFromCmd(int nLabelCmd) const
	{
		_tstring sLabel;
		if (IS_LABEL_CMD(nLabelCmd))
		{
			int pos = GET_LABEL_ID_FROM_CMD(nLabelCmd);
			int i = 1;
			for (std::list<_tstring>::const_iterator it = m_listLabels.begin();
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
	void PopulateNotebooksMenu(CMenuHandle menuNotebooks)
	{
		const CAppState::CRecentFileList& rf =  CApplication::Get().GetState().GetRecentFiles();

		m_listNotebooks.clear();
		for (CAppState::CRecentFileList::const_iterator it = rf.begin(); it != rf.end(); ++it)
		{
			if (::PathFileExists(it->GetName().c_str()))
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
	_tstring GetNotebookFromCmd(int nCmd) const
	{
		_tstring sFile;
		if (IS_NOTEBOOK_CMD(nCmd))
		{
			int pos = GET_NOTEBOOK_ID_FROM_CMD(nCmd);
			int i = 0;
			for (std::list<_tstring>::const_iterator it = m_listNotebooks.begin();
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
	void ProcessNotebookOpen(int nCmd)
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
	bool PopulateMoveToNotebooksMenu(CMenuHandle menuNotebooks)
	{
		const CAppState::CRecentFileList& rf = CApplication::Get().GetState().GetRecentFiles();

		m_listMoveToNotebooks.clear();
		for (CAppState::CRecentFileList::const_iterator it = rf.begin(); it != rf.end(); ++it)
		{
			if (::PathFileExists(it->GetName().c_str()))
				m_listMoveToNotebooks.push_back(it->GetName());
		}
		m_listMoveToNotebooks.sort();

		int pos = 0;
		int nCmd = CREATE_MOVE_TO_NOTEBOOK_CMD(0);
		bool added = false;
		for (std::list<_tstring>::iterator it = m_listMoveToNotebooks.begin(); it != m_listMoveToNotebooks.end(); ++it)
		{
			menuNotebooks.InsertMenu(pos, MF_BYPOSITION, nCmd++, it->c_str());
			if (CApplication::Get().GetDataFileName() == *it)
			{
				menuNotebooks.CheckMenuItem(pos, MF_BYPOSITION | MF_CHECKED);
				menuNotebooks.EnableMenuItem(pos, MF_BYPOSITION | MF_GRAYED);
			}
			pos++;
			added = true;
		}
		return added;
	}

	/**/
	_tstring GetMoveToNotebookFromCmd(int nCmd) const
	{
		_tstring sFile;
		if (IS_MOVE_TO_NOTEBOOK_CMD(nCmd))
		{
			int pos = GET_MOVE_NOTEBOOK_ID_FROM_CMD(nCmd);
			int i = 0;
			for (std::list<_tstring>::const_iterator it = m_listMoveToNotebooks.begin();
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
	void ProcessMoveToNotebook(int nCmd)
	{
		_tstring sFile;
		std::list<int> notes;
		
		T* pT = static_cast<T*>(this);
		pT->GetSelectedNotes(notes);

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
			ProcessSelectedNotes(notes, acMoveToNotebook, sFile.c_str());
		}
	}

	/**/
	void ProcessLabelSelected(int nCmd)
	{
		_tstring sLabel;
		std::list<int> notes;
		
		T* pT = static_cast<T*>(this);
		pT->GetSelectedNotes(notes);
		if (notes.empty())
		{
			return;
		}

		if (nCmd == ID_TNM_NEWLABEL)
		{
			CNewLabelDialog	dlg;
			dlg.m_nInitParam = (CNewLabelDialog::ipCursorPos | CNewLabelDialog::ipPopup);
			if (dlg.DoModal() == IDOK)
			{
				sLabel = dlg.m_sLabel;
			}
		}
		else
		{
			sLabel = GetLabelFromCmd(nCmd);
			_tstring label = CApplication::Get().GetNoteLabel(*notes.begin());
			if (label == sLabel)
			{
				sLabel = _T(""); // если выбрана та же метка - очистить
			}
		}
		ProcessSelectedNotes(notes, acLabel, sLabel.c_str());
	}
private:
	std::list<_tstring> m_listLabels;
	std::list<_tstring> m_listNotebooks;
	std::list<_tstring> m_listMoveToNotebooks;
};
