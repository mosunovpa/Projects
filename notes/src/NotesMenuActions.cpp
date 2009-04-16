#include "StdAfx.h"
#include "NotesMenuActions.h"

/*
CNotesMenuActions::CNotesMenuActions(void)
{
}

CNotesMenuActions::~CNotesMenuActions(void)
{
}
*/

CNotesMenuActions::iterator CNotesMenuActions::find(int nNoteId)
{
	for (CNotesMenuActions::iterator it = begin(); it != end(); ++it)
	{
		if (it->m_nNoteId == nNoteId)
		{
			return it;
		}
	}
	return end();
}

int CNotesMenuActions::GetMarkedCount() const
{
	int count = 0;
	for (CNotesMenuActions::const_iterator it = begin(); it != end(); ++it)
	{
		if (it->IsState(CNotesMenuItem::stChecked))
		{
			++count;
		}
	}
	return count;
}