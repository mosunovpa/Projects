#pragma once

class CSimpleNoteEdit : public CWindowImpl<CSimpleNoteEdit, CEdit>,
                public CEditCommands<CSimpleNoteEdit>
{
public:
	DECLARE_WND_SUPERCLASS(NULL, CEdit::GetWndClassName())
	BEGIN_MSG_MAP(CSimpleNoteEdit)
             

		CHAIN_MSG_MAP_ALT(CEditCommands<CSimpleNoteEdit>, 1)
	END_MSG_MAP()

     
};
