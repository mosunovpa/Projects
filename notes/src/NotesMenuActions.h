#pragma once

class CNotesMenuItem
{
public:
	enum States { stNone = 0, stChecked };
	enum Actions { acNone = 0, acDelete, acOpen };

	CNotesMenuItem(int nNoteId = 0, HMENU hPpopupMenu = NULL) 
		: m_nNoteId(nNoteId), m_hPpopupMenu(hPpopupMenu), m_action(acNone)
	{

	}

	int m_nNoteId;
	HMENU m_hPpopupMenu;
	States m_state;
	Actions m_action;
};

//typedef std::list<CNotesMenuItem> CNotesMenuActions;

class CNotesMenuActions : public std::list<CNotesMenuItem>
{
public:
	CNotesMenuActions::iterator find(int nNoteId);
	int GetMarkedCount() const;
};

