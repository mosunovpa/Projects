#pragma once

class CNotesView
{
public:
	enum NotesActions { acNone = 0, acDelete, acOpen, acLabel, acClipboard, acRestore, acMoveToNotebook };

	CNotesView(void) {};
	~CNotesView(void) {};

	void PopulateLabelMenu(CMenuHandle menuLabels, _tstring const& sLabel, BOOL bCheckPadio = TRUE);
	void ProcessSelectedNotes(const std::list<int>& notes, NotesActions action, LPCTSTR sParam = NULL);
	_tstring GetLabelFromCmd(int nLabelCmd);
	void PopulateNotebooksMenu(CMenuHandle menuNotebooks);
	_tstring GetNotebookFromCmd(int nCmd);
	void ProcessNotebookOpen(int nCmd);
	bool PopulateMoveToNotebooksMenu(CMenuHandle menuNotebooks);
	_tstring GetMoveToNotebookFromCmd(int nCmd);
	void ProcessMoveToNotebook(int nCmd, int nNoteId);
private:
	std::list<_tstring> m_listLabels;
	std::list<_tstring> m_listNotebooks;
	std::list<_tstring> m_listMoveToNotebooks;
};
