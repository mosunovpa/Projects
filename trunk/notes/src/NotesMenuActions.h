#pragma once

class CNotesMenuItem
{
public:
	enum States { stNone = 0x00, stChecked = 0x01, stDeleted = 0x02 };
	enum Actions { acNone = 0, acDelete, acOpen };

	CNotesMenuItem(int nNoteId = 0, HMENU hPpopupMenu = NULL) 
		: m_nNoteId(nNoteId), m_hPopupMenu(hPpopupMenu), m_action(acNone), m_state(stNone)
	{
	}

	BOOL IsState(States state) const
	{
		return (m_state & state) == state;
	}

	void SetState(States state, BOOL bAppend = TRUE)
	{
		if (bAppend)
		{
			m_state |= state;
		}
		else
		{
			m_state &= ~state;
		}
	}

	int m_nNoteId;
	HMENU m_hPopupMenu;
	Actions m_action;

private:

	UINT m_state;

};

//typedef std::list<CNotesMenuItem> CNotesMenuActions;

class CNotesMenuActions : public std::list<CNotesMenuItem>
{
public:
	CNotesMenuActions::iterator find(int nNoteId);
	int GetMarkedCount() const;
};

