#pragma once

#include "atlscintilla.h"
#include <boost/algorithm/string/trim.hpp>

class CScintillaNoteEdit : public CScintillaWindowImpl<CScintillaNoteEdit>,
                public CEditCommands<CScintillaNoteEdit>
{
public:
	DECLARE_WND_SUPERCLASS(NULL, CScintilla::GetWndClassName())
	BEGIN_MSG_MAP(CScintillaNoteEdit)
		CHAIN_MSG_MAP(CScintillaWindowImpl<CScintillaNoteEdit>)
		CHAIN_MSG_MAP_ALT(CEditCommands<CScintillaNoteEdit>, 1)
	END_MSG_MAP()

    void Create(HWND hWndParent) 
	{
		CScintillaWindowImpl<CScintillaNoteEdit>::Create(hWndParent, NULL, NULL, 
			WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | ES_MULTILINE | WS_VSCROLL, 
			WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY, 20000);
	}

	void Init()
	{
		SetCodePage(SC_CP_UTF8);//SC_CP_UTF8
	}

	void SetFontSize(LONG lHeight)
	{
	}

	void SetModify(BOOL bModified = TRUE)
	{
	}

	void GetSel(long& nStartChar, long& nEndChar) const
	{
	}

	_tstring GetText()
	{
		string s;
		int len = GetLength();
		s.resize(len + 1);
		CScintillaWindowImpl<CScintillaNoteEdit>::GetText(s.size(), &s[0]);
		boost::trim(s);
		if (s[0] == 0)
		{
			s.clear();
		}
		_tstring ws = ATOT(s.c_str(), s.size());
		return ws;
	}

	void SetText(_tstring const& text)
	{
		string s = TTOA(text.c_str(), text.size());
		CScintillaWindowImpl<CScintillaNoteEdit>::SetText(s.c_str());
	}

private:

	void SetAStyle(int style, COLORREF fore, COLORREF back, int size, const char *face) {
		StyleSetFore(style, fore);
		StyleSetBack(style, back);
		if (size >= 1)
			StyleSetSize(style, size);
		if (face) 
			StyleSetFont(style, face);
	}

};
