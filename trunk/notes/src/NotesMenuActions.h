#pragma once

class CNotesMenuItem
{
public:
	enum Actions { acNone = 0, acDeleted, asRestored };

	CNotesMenuItem(int nNoteId = 0, HMENU hPpopupMenu = NULL) 
		: m_nNoteId(nNoteId), m_hPpopupMenu(hPpopupMenu), m_action(acNone)
	{

	}

	int m_nNoteId;
	HMENU m_hPpopupMenu;
	Actions m_action;
};

//typedef std::list<CNotesMenuItem> CNotesMenuActions;

class CNotesMenuActions : public std::list<CNotesMenuItem>
{
public:
	CNotesMenuActions::iterator find(int nNoteId);
};

